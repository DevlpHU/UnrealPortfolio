// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "UIChatText_Game.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class GAME_API UUIChatText_Game : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Chat;
};
