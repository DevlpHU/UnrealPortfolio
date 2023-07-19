// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Engine//DataTable.h"
#include "Engine/GameInstance.h"
#include "GameInstance_Game.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPlayerData() : Level(1), MaxHP(100.0f), Attack(10.0f), NextExp(10) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

USTRUCT(BlueprintType)
struct FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterData() : Level(1), MaxHP(100.0f), Attack(20.0f), DropExp(10) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;
};

USTRUCT(BlueprintType)
struct FItemBaseData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemBaseData() : Name(FName("")), Type(EItemType::None), Icon(nullptr), Attack(0.0f), HP(0.0f), BuyGold(0), Class(nullptr) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	EItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 BuyGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UClass* Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText Information;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	
public:
	FItemData() : BaseData(FItemBaseData()), Count(1), IsEquip(false) {};
	FItemData(FItemBaseData Base) : BaseData(Base), Count(1), IsEquip(false) {};
	void Use(class APlayer_Game& Player, int32 SlotNum);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FItemBaseData BaseData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	int32 Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	bool IsEquip;
};

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FQuestData() : Name(FText()), Story(FText()), Require(FName()), RequireCount(1), ProgressType(EQuestProgressType::Waiting), QuestType(EQuestType::None), ItemList() {}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FText Story;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName Require;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	int32 RequireCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	EQuestProgressType ProgressType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TSubclassOf<class AItem_Game>> ItemList;
};

struct FKillMap
{
public:
	FKillMap() { Map.Empty(); }

	bool IsKillingData(FName Name) { return Map.Contains(Name); }
	int32 GetKillingCount(FName Name) const { return Map[Name]; }
	void AddKillingData(FName Name, int32 Count);
	void AddKillingCount(FName Name) { Map[Name]++; }

private:
	TMap<FName, int32> Map;
};

struct FQuestMap
{
public:
	FQuestMap() { Map.Empty(); }

	bool IsQuestName(FName Name) { return Map.Contains(Name); }
	bool IsQuestSolved(FName Name) const { return Map[Name]; }
	void InsertQuestName(FName Name) { Map.Emplace(Name, false); }
	void SetQuestSolved(FName Name, bool Solved) { Map[Name] = Solved; }
	
	bool FindQuestSolved(FName Name);

private:
	TMap<FName, bool> Map;
};

/**
 * 
 */
UCLASS()
class GAME_API UGameInstance_Game : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameInstance_Game();

	virtual void Init() override;

	FPlayerData*  GetPlayerData(int32 Level);
	FMonsterData* GetMonsterData(int32 Level);
	FItemBaseData* GetItemBaseData(FName NewName);
	FQuestData* GetQuestData(FText QuestName);

	void QuestMapInit();

	UFUNCTION(BlueprintCallable)
	void SaveInventory(APlayerController_Game* InventoryOwner);

	UFUNCTION(BlueprintCallable)
	void LoadInventory(APlayerController_Game* InventoryOwner);

	UFUNCTION(BlueprintCallable)
	void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable)
	void StopLoadingScreen();
	
	FKillMap* KillMap;
	FQuestMap* QuestMap;

	UPROPERTY(BlueprintReadWrite)
	FText Name;

private:
	UPROPERTY()
	class UDataTable* PlayerData;

	UPROPERTY()
	class UDataTable* MonsterData;

	UPROPERTY()
	class UDataTable* ItemBaseData;

	UPROPERTY()
	class UDataTable* QuestData;

	UPROPERTY()
	TArray<FItemData> Inventory;

	UPROPERTY()
	int32 Gold;
};
