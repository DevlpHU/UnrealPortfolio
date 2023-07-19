// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI_Game.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "PlayerController_Game.h"
#include "ItemDataArray_Game.h"
#include "GameInstance_Game.h"

void UInventoryUI_Game::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	Init();
}

void UInventoryUI_Game::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UInventoryUI_Game::Init()
{
	Button_Inventory->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::NoDrawType;

	Text_Inventory->Font.Size = 20;
	Text_Inventory->SetText(FText::FromString(TEXT("인벤토리")));

	ListView_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	SetGoldText(0);
}

void UInventoryUI_Game::Refresh()
{
	ListView_Inventory->ClearListItems();

	auto playerController = Cast<APlayerController_Game>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto& items = playerController->GetInventory();
	int32 itemCount = items.Num();

	int32 cnt = (itemCount % 5) == 0 ? 0 : 1;
	for (int32 i = 0; i < itemCount / 5 + cnt; ++i)
	{
		TArray<FItemData> itemArray;
		for (int32 j = 0; j < 5; ++j)
		{
			if (i * 5 + j < itemCount)
			{
				itemArray.Emplace(items[i * 5 + j]);
			}
		}
		
		auto listItem = NewObject<UItemDataArray_Game>();
		listItem->Setting(itemArray);
		listItem->ListNum = i;
		listItem->Type = ESlotType::Inventory;
		ListView_Inventory->AddItem(listItem);
	}

}

void UInventoryUI_Game::SetGoldText(int32 Gold)
{
	Text_Gold->SetText(FText::FromString(FString::Printf(TEXT("보유 금액 : %dG"), Gold)));
}
