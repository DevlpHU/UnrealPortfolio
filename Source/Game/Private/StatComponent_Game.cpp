// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent_Game.h"
#include "GameInstance_Game.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Player_Game.h"
#include "StatComponent_Game.h"

// Sets default values for this component's properties
UStatComponent_Game::UStatComponent_Game()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
	Level = 1;
	StatType = EStatComponentType::Undefined;

	EquipHP = 0.0f;
	EquipAttack = 0.0f;
	
}


// Called when the game starts
void UStatComponent_Game::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatComponent_Game::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UStatComponent_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatComponent_Game, Level);
	DOREPLIFETIME(UStatComponent_Game, CurrentHP);
	DOREPLIFETIME(UStatComponent_Game, EquipHP);
	DOREPLIFETIME(UStatComponent_Game, TotalHP);
	DOREPLIFETIME(UStatComponent_Game, EquipAttack);
	DOREPLIFETIME(UStatComponent_Game, TotalAttack);
	DOREPLIFETIME(UStatComponent_Game, Exp);
	DOREPLIFETIME(UStatComponent_Game, CurrentExp);
}


void UStatComponent_Game::ClientOnHPChanged_Implementation()
{
	OnHPChanged.ExecuteIfBound();
}

void UStatComponent_Game::ClientOnExpChanged_Implementation()
{
	OnExpChanged.ExecuteIfBound();
}

void UStatComponent_Game::ClientOnLevelUp_Implementation()
{
	OnLevelUpDele.ExecuteIfBound();
}

void UStatComponent_Game::OnRep_CurrentHP()
{
	if (StatType == EStatComponentType::Player)
	{
		ClientOnHPChanged();
	}
}

void UStatComponent_Game::OnRep_CurrentExp()
{
	if (StatType == EStatComponentType::Player)
	{
		ClientOnExpChanged();
	}
}

void UStatComponent_Game::OnRep_Level()
{
	if (StatType == EStatComponentType::Player)
	{
		ClientOnLevelUp();
	}
}

void UStatComponent_Game::SetNewLevel(int32 NewLevel)
{
	auto gameInstance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (IsValid(gameInstance))
	{
		switch (StatType)
		{
			case EStatComponentType::Player:
			{
				CurrentPlayerStatData = gameInstance->GetPlayerData(NewLevel);
				if (CurrentPlayerStatData != nullptr)
				{
					Level = NewLevel;
					TotalHP = CurrentPlayerStatData->MaxHP + EquipHP;
					CurrentHP = TotalHP;
					TotalAttack = CurrentPlayerStatData->Attack + EquipAttack;
					CurrentExp = 0;
					Exp = CurrentPlayerStatData->NextExp;
				}
				break;
			}
			case EStatComponentType::Monster:
			{
				CurrentMonsterStatData = gameInstance->GetMonsterData(NewLevel);
				if (CurrentMonsterStatData != nullptr)
				{
					Level = NewLevel;
					TotalHP = CurrentMonsterStatData->MaxHP + EquipHP;
					CurrentHP = TotalHP;
					TotalAttack = CurrentMonsterStatData->Attack + EquipAttack;
					Exp = CurrentMonsterStatData->DropExp;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		
	}
}

void UStatComponent_Game::SetDamage(float NewDamage, AController* EventInstigator)
{
	if (IsStatDataValid() == false)
	{
		return;
	}

	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, TotalHP);
	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();

		if (StatType == EStatComponentType::Monster)
		{
			APlayer_Game* player = Cast<APlayer_Game>(EventInstigator->GetPawn());
			if (player == nullptr) return;

			player->PlayerStat->AddCurrentExp(Exp);
			for (int32 i = 0; i < player->CurPlayerCount; ++i)
			{
				player->Party[i]->PlayerStat->AddCurrentExp(Exp);
			}
		}
	}
	
}

void UStatComponent_Game::SetEquipAttack(float Attack)
{
	float baseAttack = TotalAttack - EquipAttack;
	EquipAttack = Attack;
	TotalAttack = baseAttack + Attack;
}

void UStatComponent_Game::SetEquipHP(float HP)
{
	float baseMaxHP = TotalHP - EquipHP;
	EquipHP = HP;
	TotalHP = baseMaxHP + HP;
}

void UStatComponent_Game::AddCurrentExp(int32 AddExp)
{
	int32 exp = CurrentExp + AddExp;
	if (exp < Exp)
	{
		CurrentExp = exp;
		return;
	}

	SetNewLevel(Level + 1);
}

void UStatComponent_Game::Revive()
{
	CurrentHP = TotalHP;
}



