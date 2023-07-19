// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "GameFramework/PlayerController.h"
#include "GameInstance_Game.h"
#include "PlayerController_Game.generated.h"

class UMainUI_Game;
class ANPC_Game;
class APlayer_Game;
struct FItemData;

DECLARE_DELEGATE(FOnInventoryDelegate);

/**
 * 
 */
UCLASS()
class GAME_API APlayerController_Game : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerController_Game();

	virtual void OnPossess(APawn* aPawn) override;

	void SetInputGameModes(bool IsGameMode = true, bool IsUIMode = false, bool IsShowCursor = false);

	UFUNCTION(BlueprintCallable)
	UMainUI_Game* GetHUDWidget() const;

	TArray<FItemData>& GetInventory();

	void PutItemToInventory(FItemData ItemData);
	void OrganizeInventory();

	void SetInteractedNPC(ANPC_Game* NPC) { InteractedNPC = NPC; }
	ANPC_Game* GetInteractedNPC() { return InteractedNPC; }

	void ShowPartyUI(APlayer_Game* AskPlayer);
	
	const TArray<FItemData>& GetInventoryData() { return Inventory; }
	void SetInventoryEmpty() { Inventory.Empty(); }

	int32 GetGold() { return Gold; }
	void SetGold(int32 Money) { Gold = Money; }

	//RPC
	UFUNCTION(Server, Reliable)
	void ServerDestroy(AActor* Actor);

	UFUNCTION(Server, Reliable)
	void ServerSendMessage(const FText& Text);

	UFUNCTION(Client, Reliable)
	void ClientUpdateChatting(const FText& Text);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetIsReady(bool Ready);

	FOnInventoryDelegate OnInventoryDele;

protected: 
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndType) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainUI_Game> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUIAskParty_Game> PartyWidgetClass;

private:
	UPROPERTY()
	UMainUI_Game* HUDWidget;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UGameInstance_Game* GameInstance;

	//FItemData, Count
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FItemData> Inventory;

	//Item 존재 여부와 인덱스 확인을 위함
	//FItemData->Name, SlotNum
	TMultiMap<FName, int32> ItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Gold;

	UPROPERTY(VisibleAnywhere, Category = "NPC")
	ANPC_Game* InteractedNPC;

	TWeakObjectPtr<APlayer_Game> PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool IsReady = false;
};
