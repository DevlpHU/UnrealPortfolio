// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InventoryUISlots_Game.generated.h"

class UUISlot_Game;
/**
 * 
 */
UCLASS()
class GAME_API UInventoryUISlots_Game : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UUISlot_Game* GetSlotWidget(int32 Number);
	
protected:
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	void Init();

	UPROPERTY()
	TArray<UUISlot_Game*> SlotArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "List")
	int32 ListNum;
};
