// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "UIAskParty_Game.generated.h"

class UButton;
class APlayer_Game;
/**
 * 
 */
UCLASS()
class GAME_API UUIAskParty_Game : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool SetAskPlayer(APlayer_Game* AskPlyr);

protected:
	virtual void NativeConstruct();
	void ControllerInputInit();

	UFUNCTION()
	void Accept();

	UFUNCTION()
	void Refuse();

	UPROPERTY(meta=(BindWidget))
	UButton* Button_Accept;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Button_Refuse;

	APlayer_Game* AskPlayer;
};
