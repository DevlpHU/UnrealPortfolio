// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Game.h"
#include "Engine/Texture2D.h"
#include "Item_Game.h"
#include "Player_Game.h"
#include "StatComponent_Game.h"
#include "PlayerController_Game.h"
#include "GameLoadingScreen.h"

UGameInstance_Game::UGameInstance_Game()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PLAYER(TEXT("DataTable'/Game/GameData/PlayerData.PlayerData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MONSTER(TEXT("DataTable'/Game/GameData/MonsterData.MonsterData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEMBASE(TEXT("DataTable'/Game/GameData/ItemBaseData.ItemBaseData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QUEST(TEXT("DataTable'/Game/GameData/QuestData.QuestData'"));

	if (DT_PLAYER.Succeeded())		PlayerData	= DT_PLAYER.Object;
	if (DT_MONSTER.Succeeded())		MonsterData = DT_MONSTER.Object;
	if (DT_ITEMBASE.Succeeded())	ItemBaseData = DT_ITEMBASE.Object;
	if (DT_QUEST.Succeeded())		QuestData = DT_QUEST.Object;

	Name = FText::GetEmpty();
}

void UGameInstance_Game::Init()
{
	Super::Init();

	KillMap = new FKillMap(); KillMap->AddKillingData(FName("Boss"), 1);
	QuestMap = new FQuestMap();

	QuestMapInit();
}

FPlayerData* UGameInstance_Game::GetPlayerData(int32 Level)
{
	return PlayerData->FindRow<FPlayerData>(*FString::FromInt(Level), *FString::FromInt(Level));
}

FMonsterData* UGameInstance_Game::GetMonsterData(int32 Level)
{
	return MonsterData->FindRow<FMonsterData>(*FString::FromInt(Level), *FString::FromInt(Level));
}

FItemBaseData* UGameInstance_Game::GetItemBaseData(FName NewName)
{
	return ItemBaseData->FindRow<FItemBaseData>(*NewName.ToString(), *NewName.ToString());
}

FQuestData* UGameInstance_Game::GetQuestData(FText QuestName)
{
	return QuestData->FindRow<FQuestData>(*QuestName.ToString(), *QuestName.ToString());
}

void UGameInstance_Game::QuestMapInit()
{
	TArray<FQuestData*> Array;
	QuestData->GetAllRows<FQuestData>(FString("Name"), Array);

	for (auto quest : Array)
	{
		QuestMap->InsertQuestName(FName(*(quest->Name).ToString()));
	}
}

void UGameInstance_Game::SaveInventory(APlayerController_Game* InventoryOwner)
{
	Inventory.Empty();
	for (auto Item : InventoryOwner->GetInventoryData())
	{
		Inventory.Emplace(Item);
	}

	Gold = InventoryOwner->GetGold();
}

void UGameInstance_Game::LoadInventory(APlayerController_Game* InventoryOwner)
{
	InventoryOwner->SetInventoryEmpty();
	for (auto Item : Inventory)
	{
		InventoryOwner->PutItemToInventory(Item);
	}
	
	InventoryOwner->SetGold(Gold);
}

void UGameInstance_Game::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	IGameLoadingScreenModule& LoadingScreenModule = IGameLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UGameInstance_Game::StopLoadingScreen()
{
	IGameLoadingScreenModule& LoadingScreenModule = IGameLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

//무기 장착시 스폰, 소비 아이템 전부 사용 시 Type 변경
void FItemData::Use(APlayer_Game& Player, int32 SlotNum)
{
	auto playerController = Cast<APlayerController_Game>(Player.GetController());

	switch (BaseData.Type)
	{
	case EItemType::None:
		break;
	case EItemType::Potion:
	{
		if (Count > 0) {
			Player.ServerSetDamage(-BaseData.HP);
			Count--;
		}

		if (Count <= 0)
		{
			playerController->OrganizeInventory();
			if (Player.CurEquipWeaponSlot > SlotNum) Player.CurEquipWeaponSlot--;
		}
	}
	break;
	case EItemType::Weapon:
	{
		float attack = 0.0f, hP = 0.0f;
		if (IsEquip == false) {	//새로운 아이템 장착 시
			attack = BaseData.Attack;
			hP = BaseData.HP;
			Player.ServerEquipWeapon(BaseData.Class);
			if (Player.CurEquipWeaponSlot != -1) //이전에 장착 중인 아이템 존재 시 인벤토리 IsEquip 변경
				playerController->GetInventory()[Player.CurEquipWeaponSlot].IsEquip = false;
			Player.CurEquipWeaponSlot = SlotNum;
		}
		else {
			Player.ServerUnEquipWeapon();
			Player.CurEquipWeaponSlot = -1;
		}

		Player.ServerSetEquipAttack(attack);
		Player.ServerSetEquipHP(hP);
		IsEquip = !IsEquip;
	}
	break;
	default:
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Item Type Error!"));
	}
	}

	playerController->OnInventoryDele.Execute();
}

void FKillMap::AddKillingData(FName Name, int32 Count)
{
	Map.Emplace(Name, Count);
}

bool FQuestMap::FindQuestSolved(FName Name)
{
	bool* result = Map.Find(Name);
	if (result == nullptr) return false;
	else return *result;
}