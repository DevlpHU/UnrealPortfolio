// Fill out your copyright notice in the Description page of Project Settings.


#include "UIInformation_Game.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "GameInstance_Game.h"

void UUIInformation_Game::SetTextName(FText Name)
{
	Text_Name->SetText(Name);
}

void UUIInformation_Game::SetTextInformation(FText Information)
{
	Text_Information->SetText(Information);
}

void UUIInformation_Game::SetTextStat(float HP, float Attack)
{
	if (Attack < KINDA_SMALL_NUMBER)
	{
		Text_Stat->SetText(FText::FromString(FString::Printf(TEXT("회복체력 : %.1f"), HP)));
	}
	else
	{
		Text_Stat->SetText(FText::FromString(FString::Printf(TEXT("공격력 : %.1f \n추가체력 : %.1f"), Attack, HP)));
	}
}

void UUIInformation_Game::Setting(FItemBaseData& Data)
{
	SetTextName(FText::FromName(Data.Name));
	SetTextInformation(Data.Information);
	SetTextStat(Data.HP, Data.Attack);
}