// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "UIInformation_Game.generated.h"

class UTextBlock;
class UMultiLineEditableTextBox;
/**
 * 
 */
UCLASS()
class GAME_API UUIInformation_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTextName(FText Name);
	void SetTextInformation(FText Information);
	void SetTextStat(float HP, float Attack = 0.0f);
	
	void Setting(struct FItemBaseData& Data);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Stat;

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* Text_Information;
};
