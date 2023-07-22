// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI_Game.h"
#include "InventoryUI_Game.h"
#include "UIChatting_Game.h"
#include "StatComponent_Game.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player_Game.h"
#include "PartyPlayerUI_Game.h"

void UMainUI_Game::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UMainUI_Game::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UMainUI_Game::Init()
{
	Inventory_Widget->SetVisibility(ESlateVisibility::Collapsed);

	Player = Cast<APlayer_Game>(GetOwningPlayerPawn());
	if (Player == nullptr)
	{
		return;
	}

	Player->OnChangePartyPlayerDele.BindUObject(this, &UMainUI_Game::UpdatePartyUI);
	Player_Widgets.Init(nullptr, Player->MaxPartyNumber);

	for (int32 i = 0; i < Player->MaxPartyNumber; ++i)
	{
		FName widgetName = FName(*(FString("Player_Widget") + FString::FromInt(i + 1)));
		Player_Widgets[i] = Cast<UPartyPlayerUI_Game>(GetWidgetFromName(widgetName));
		Player_Widgets[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainUI_Game::UpdateAll()
{
	CurrentPlayerStat->OnHPChanged.ExecuteIfBound();
	CurrentPlayerStat->OnExpChanged.ExecuteIfBound();
	CurrentPlayerStat->OnLevelUpDele.ExecuteIfBound();
	UpdatePlayerImage();
	Player->OnChangePartyPlayerDele.ExecuteIfBound();
}

void UMainUI_Game::UpdatePlayerHPBar()
{
	float ratio = CurrentPlayerStat->GetHPRatio();
	ProgressBar_HP->SetPercent(ratio);

	FString curHpStr = FString::FromInt(CurrentPlayerStat->GetCurrentHP());
	FString maxHpStr = FString::FromInt(CurrentPlayerStat->GetTotalHP());

	Text_HP->SetText(FText::FromString(curHpStr + FString(" / ") + maxHpStr));
}

void UMainUI_Game::UpdatePlayerExpBar()
{
	float ratio = CurrentPlayerStat->GetExpRato();
	ProgressBar_Exp->SetPercent(ratio);

	FString curExpStr = FString::FromInt(CurrentPlayerStat->GetCurrentExp());
	FString maxExpStr = FString::FromInt(CurrentPlayerStat->GetExp());
	FString ratioStr = FString(" (") + FString::SanitizeFloat(ratio * 100) + FString("%)");

	Text_Exp->SetText(FText::FromString(curExpStr + FString(" / ") + maxExpStr + ratioStr));
}

void UMainUI_Game::UpdatePlayerImage()
{
	if (Player == nullptr)
	{
		return;
	}

	Image_Player->SetBrushFromTexture(Player->Symbol);
}

void UMainUI_Game::UpdatePartyUI()
{
	for (int32 i = 0; i < Player->MaxPartyNumber; ++i)
	{
		if (Player->Party[i] == nullptr) 
		{
			Player_Widgets[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Player_Widgets[i]->SetVisibility(ESlateVisibility::Visible);
			Player_Widgets[i]->SetPlayerInfor(Player->Party[i]);
		}
	}
}

void UMainUI_Game::UpdateLevelText()
{
	FString prefixStr = FString("LV.");
	FString LevelStr = FString::FromInt(CurrentPlayerStat->GetLevel());

	Text_Level->SetText(FText::FromString(prefixStr + LevelStr));
}

void UMainUI_Game::BindPlayerStatData(UStatComponent_Game* PlayerStat)
{
	CurrentPlayerStat = PlayerStat;

	CurrentPlayerStat->OnHPChanged.BindUObject(this, &UMainUI_Game::UpdatePlayerHPBar);
	CurrentPlayerStat->OnExpChanged.BindUObject(this, &UMainUI_Game::UpdatePlayerExpBar);
	CurrentPlayerStat->OnLevelUpDele.BindUObject(this, &UMainUI_Game::UpdateLevelText);
}

UUIChatting_Game* UMainUI_Game::GetChatting_Widget()
{
	return Chatting_Widget;
}

UInventoryUI_Game* UMainUI_Game::GetInventoryWidget()
{
	return Inventory_Widget;
}
