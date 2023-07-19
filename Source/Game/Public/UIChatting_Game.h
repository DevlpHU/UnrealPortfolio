// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "UIChatting_Game.generated.h"

class UButton;
class UScrollBox;
class UEditableText;

/**
 * 
 */
UCLASS()
class GAME_API UUIChatting_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	FText ConfigureText(const FText& Msg);
	void UpdateChatting(FText Chat);
	void OnChat();

	UFUNCTION()
	void SendBtnClick();

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Send;

protected:
	UFUNCTION()
	void Committed(const FText& Text, ETextCommit::Type CommitMethod);

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox_Chat;

	UPROPERTY(meta = (BindWidget))
	UEditableText* EditableText_Chat;
};
