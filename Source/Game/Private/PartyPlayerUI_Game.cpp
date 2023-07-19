// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyPlayerUI_Game.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Player_Game.h"
#include "StatComponent_Game.h"

void UPartyPlayerUI_Game::SetPlayerInfor(APlayer_Game* Player)
{
	Image_Symbol->SetBrushFromTexture(Player->Symbol);
	Text_Name->SetText(Player->GetPlayerName());
	ProgressBar_HP->SetPercent(Player->PlayerStat->GetHPRatio());
}