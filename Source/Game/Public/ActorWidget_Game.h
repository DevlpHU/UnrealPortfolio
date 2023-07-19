// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "ActorWidget_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UActorWidget_Game : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetTextNameText(FText name);

	UFUNCTION(BlueprintCallable)
	void SyncProgressBar();

	UFUNCTION(BlueprintCallable)
	void SetPercentHPBar(float Percent);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextName;
	
	UPROPERTY()
	class UProgressBar* HPBar;
};
