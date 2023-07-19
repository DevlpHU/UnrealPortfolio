// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "PartyPlayerUI_Game.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class APlayer_Game;
/**
 * 
 */
UCLASS()
class GAME_API UPartyPlayerUI_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPlayerInfor(APlayer_Game* Player);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_Symbol;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Name;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_HP;
};
