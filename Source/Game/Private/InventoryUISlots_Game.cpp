// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUISlots_Game.h"
#include "UISlot_Game.h"
#include "ItemDataArray_Game.h"
#include "GameInstance_Game.h"
#include "PlayerController_Game.h"

void UInventoryUISlots_Game::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	Init();
}

void UInventoryUISlots_Game::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UInventoryUISlots_Game::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UItemDataArray_Game* itemArray = Cast<UItemDataArray_Game>(ListItemObject);

	if (IsValid(itemArray))
	{
		auto& itemDataArray = itemArray->ItemDataArray;

		ListNum = itemArray->ListNum;
		for (int32 i = 0; i < SlotCount; ++i)
		{
			if (i < itemDataArray.Num())
			{
				SlotArray[i]->SetSlotType(itemArray->Type);
				SlotArray[i]->SlotNum = (ListNum * 5) + i;
				SlotArray[i]->Refresh(itemDataArray[i]);
			}
			else
			{
				SlotArray[i]->DefaultRefresh();
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("ListView Add Error<itemArray nullptr>"));
	}
}

void UInventoryUISlots_Game::Init()
{
	SlotArray.Init(nullptr, SlotCount);
	for (int32 i = 0; i < 5; ++i)
	{
		SlotArray[i] = Cast<UUISlot_Game>(GetWidgetFromName(FName(FString::Printf(TEXT("Slot_Widget%d"), i + 1))));
		SlotArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UUISlot_Game* UInventoryUISlots_Game::GetSlotWidget(int32 Number)
{
	return SlotArray[Number];
}

