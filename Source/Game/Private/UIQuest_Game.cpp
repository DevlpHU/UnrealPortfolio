// Fill out your copyright notice in the Description page of Project Settings.


#include "UIQuest_Game.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "NPC_Game.h"
#include "GameInstance_Game.h"
#include "Item_Game.h"
#include "ItemDataArray_Game.h"
#include "PlayerController_Game.h"

void UUIQuest_Game::Init(ANPC_Game* NPC)
{
	Button_Accept->OnClicked.AddDynamic(this, &UUIQuest_Game::AcceptQuest);
	Button_Refuse->OnClicked.AddDynamic(this, &UUIQuest_Game::RefuseQuest);

	Instance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
	Data = *(Instance->GetQuestData(NPC->QuestName));

	SetTextName(Data.Name);
	SetTextStory(Data.Story);
	SetNameRequire(Data.Require);
	SetDataList(Data.ItemList);
	RegisterToQuestItem();

	NPC->SetItemList(Data.ItemList);
}

void UUIQuest_Game::SetVisibilityToggle()
{
	if (GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Collapsed);
}

void UUIQuest_Game::SetTextName(FText Name)
{
	Text_QuestName->SetText(Name);
}

void UUIQuest_Game::SetTextStory(FText Story)
{
	Text_QuestStory->SetText(Story);
}

void UUIQuest_Game::SetNameRequire(FName Require)
{
	int32 count = 0;
	if (Instance->KillMap->IsKillingData(Require)) 
		count = Instance->KillMap->GetKillingCount(Require);

	Text_Require->SetText(FText::FromString(FString::Printf(TEXT("%s (%d/%d)"), *Require.ToString(), count, Data.RequireCount)));
}

void UUIQuest_Game::RegisterToQuestItem()
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
		listItem->Type = ESlotType::Quest;
		ListView_QuestItem->AddItem(listItem);
	}
}

void UUIQuest_Game::SetDataList(TArray<TSubclassOf<AItem_Game>>& Items)
{
	for (auto item : Items)
	{
		auto itemData = FItemData(*(Instance->GetItemBaseData(Cast<AItem_Game>(item.GetDefaultObject())->Name)));
		DataList.Emplace(itemData);
	}
}

bool UUIQuest_Game::IsComplete()
{
	if (Data.QuestType == EQuestType::Killing)
	{
		auto killMap = Instance->KillMap;
		if (killMap->IsKillingData(Data.Require) == true)
		{
			if (killMap->GetKillingCount(Data.Require) >= Data.RequireCount) return true;
		}
		else return false;
	}
	
	return false;
}

void UUIQuest_Game::Progress()
{
	auto& progressType = Data.ProgressType;

	switch (progressType)
	{
	case EQuestProgressType::Waiting:
	{
		SetVisibilityToggle();
	}
	break;
	case EQuestProgressType::Proceeding:
	{
		if (IsComplete() == true)
		{
			progressType = EQuestProgressType::Complete;
		}
		else
		{
			SetVisibilityToggle();
			if(IsValid(Button_Accept)) Button_Accept->RemoveFromParent();
			if(IsValid(Button_Refuse)) Button_Refuse->RemoveFromParent();
			break;
		}
	}
	case EQuestProgressType::Complete:
	{
		for (auto itemClass : Data.ItemList)
		{
			auto itemBaseData = Instance->GetItemBaseData(Cast<AItem_Game>(itemClass->GetDefaultObject())->Name);
			Cast<APlayerController_Game>(GetOwningPlayer())->PutItemToInventory(FItemData(*itemBaseData));
		}
		progressType = EQuestProgressType::End;
	case EQuestProgressType::End:
	{
		Instance->QuestMap->SetQuestSolved(FName(*(Data.Name.ToString())), true);
	}
	break;
	}
	}
}

void UUIQuest_Game::AcceptQuest()
{
	Data.ProgressType = EQuestProgressType::Proceeding;
	SetVisibility(ESlateVisibility::Collapsed);

	Cast<APlayerController_Game>(GetOwningPlayer())->GetInteractedNPC()->SetIsInteraction(false);
	Cast<APlayerController_Game>(GetOwningPlayer())->SetInputGameModes();
}

void UUIQuest_Game::RefuseQuest()
{
	Cast<APlayerController_Game>(GetOwningPlayer())->GetInteractedNPC()->RemoveUISetting();
}

