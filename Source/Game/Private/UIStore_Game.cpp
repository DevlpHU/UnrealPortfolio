// Fill out your copyright notice in the Description page of Project Settings.


#include "UIStore_Game.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "ItemDataArray_Game.h"
#include "Item_Game.h"

void UUIStore_Game::Init(TArray<TSubclassOf<AItem_Game> >& Items)
{
	SetDataList(Items);
	RegisterToStore();
}

void UUIStore_Game::SetTextName(FText Name)
{
	Text_Store->SetText(Name);
}

void UUIStore_Game::RegisterToStore()
{
	int32 itemCount = DataList.Num();
	
	int32 cnt = (itemCount % 5) == 0 ? 0 : 1;
	for (int32 i = 0; i < itemCount / 5 + cnt; ++i)
	{
		TArray<FItemData> itemArray;
		for (int32 j = 0; j < 5; ++j)
		{
			if (i * 5 + j < itemCount)
			{
				itemArray.Emplace(DataList[j]);
			}
		}

		auto listItem = NewObject<UItemDataArray_Game>();
		listItem->Setting(itemArray);
		listItem->ListNum = i;
		listItem->Type = ESlotType::Store;
		ListView_StoreSlots->AddItem(listItem);
	}
}

void UUIStore_Game::SetTextSaleGold(int32 Gold)
{
	Text_Gold->SetText(FText::FromString(FString::Printf(TEXT(" 판매 금액 : %d"), Gold)));
}

void UUIStore_Game::SetDataList(TArray<TSubclassOf<AItem_Game> >& Items)
{
	auto instance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (auto item : Items)
	{
		auto itemData = FItemData(*(instance->GetItemBaseData(Cast<AItem_Game>(item.GetDefaultObject())->Name)));
		DataList.Emplace(itemData);
	}
}
