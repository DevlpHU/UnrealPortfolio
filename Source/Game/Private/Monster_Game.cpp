// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Game.h"
#include "Net/UnrealNetwork.h"
#include "StatComponent_Game.h"
#include "AIController_Game.h"
#include "AnimInstance_Game.h"
#include "DamageRenderActor_Game.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Player_Game.h"

// Sets default values
AMonster_Game::AMonster_Game()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MonsterStat = CreateDefaultSubobject<UStatComponent_Game>(TEXT("MONSTERSTAT"));
	MonsterStat->SetStatDataType(EStatComponentType::Monster);
	
	static ConstructorHelpers::FClassFinder<ADamageRenderActor_Game> DAMAGEACTOR_C(TEXT("Blueprint'/Game/Blueprints/DamageTextActor_BP.DamageTextActor_BP_C'"));
	if (DAMAGEACTOR_C.Succeeded())	DamageActorClass = DAMAGEACTOR_C.Class;

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	MonsterStat->SetIsReplicated(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMonster_Game::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster_Game::SpawnDamageText_Implementation(float Damage)
{
	FVector loc = GetActorLocation();

	auto DamageTextActor = GetWorld()->SpawnActor<ADamageRenderActor_Game>(DamageActorClass, loc, FRotator::ZeroRotator);
	DamageTextActor->SetDamageText(Damage);
}

// Called every frame
void AMonster_Game::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonster_Game::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UAnimInstance_Game>(GetMesh()->GetAnimInstance());
	AnimInstance->OnAttackCheckDele.AddUObject(this, &AMonster_Game::_AttackCheck);
	AnimInstance->CharStateChangedDele.BindUObject(this, &AMonster_Game::ServerSetMonsterState);

	MonsterStat->OnHPIsZero.AddLambda([this]()->void {
		ServerSetMonsterState(ECurCharacterState::DeadStart);
	});
}

float AMonster_Game::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	MonsterStat->SetDamage(DamageAmount, EventInstigator);
	SpawnDamageText(DamageAmount);

	return DamageAmount;
}

// Called to bind functionality to input
void AMonster_Game::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonster_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonster_Game, CurMonsterState);
}

void AMonster_Game::MulticastDead_Implementation()
{
	if (!IsValid(AnimInstance)) return;

	AnimInstance->PlayDeadMontage();
}

void AMonster_Game::ServerSetMonsterState_Implementation(ECurCharacterState State)
{
	CurMonsterState = State;
	SetState();
}

void AMonster_Game::SetState()
{
	//¼­¹ö
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		AAIController_Game* AIController = Cast<AAIController_Game>(Controller);
		if (AIController == nullptr) return;

		switch (CurMonsterState)
		{
		case ECurCharacterState::Revive:
		{
			MonsterStat->Revive();
			SetActorEnableCollision(true);
			SetCanBeDamaged(true);
			AIController->RestartAI();
			CurMonsterState = ECurCharacterState::Idle;
		}
		break;
		case ECurCharacterState::DeadStart:
		{
			AIController->StopAI();
			MulticastDead();
			SetActorEnableCollision(false);
			SetCanBeDamaged(false);
		}
		break;
		}
	}
	else
	{
		switch (CurMonsterState)
		{
		case ECurCharacterState::Revive:
		case ECurCharacterState::Idle:
		{
			GetMesh()->SetHiddenInGame(false);
		}
		break;
		case ECurCharacterState::DeadEnd:
		{
			GetMesh()->SetHiddenInGame(true);
		}
		break;
		}
	}
}

void AMonster_Game::OnRep_State()
{
	SetState();
}

void AMonster_Game::Respawn()
{
	if (GetWorldTimerManager().IsTimerActive(RespawnTimerHandle) == true) return;

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMonster_Game::_StateToRevive, 10.f);
}

void AMonster_Game::MulitcastSkill_Implementation(int32 Num)
{
	if (!IsValid(AnimInstance)) return;

	AnimInstance->PlaySkillMontage(Num);

	if (CurMonsterState < ECurCharacterState::DeadStart)
	{
		ServerSetMonsterState(ECurCharacterState::Attacking);
	}
}

void AMonster_Game::MulitcastAttack_Implementation()
{
	if (!IsValid(AnimInstance)) return;

	AnimInstance->PlayAttackMontage();

	if (CurMonsterState < ECurCharacterState::DeadStart)
	{
		ServerSetMonsterState(ECurCharacterState::Attacking);
	}
}

void AMonster_Game::_AttackCheck()
{
	if (GetLocalRole() != ENetRole::ROLE_Authority) return;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);

	if (bResult)
	{
		for(auto HitResult : HitResults)
		{ 
			auto hitActor = Cast<APlayer_Game>(HitResult.Actor);
		
			if (IsValid(hitActor))
			{
				FDamageEvent DamageEvent;
				hitActor->TakeDamage(MonsterStat->GetTotalAttack(), DamageEvent, GetController(), this);
				return;
			}
		}
	}
}

void AMonster_Game::_StateToRevive()
{
	ServerSetMonsterState(ECurCharacterState::Revive);
}
