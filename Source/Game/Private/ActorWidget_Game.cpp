// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorWidget_Game.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UActorWidget_Game::SetTextNameText(FText name)
{
	TextName->SetText(name);
}

void UActorWidget_Game::SyncProgressBar()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_HP")));
}

void UActorWidget_Game::SetPercentHPBar(float Percent)
{
	HPBar->SetPercent(Percent);
}
