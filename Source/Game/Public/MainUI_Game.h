// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "MainUI_Game.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class UEditableText;
class UButton;
class UInventoryUI_Game;
class UCanvasPanel;
class UUIChatting_Game;

/**
 * 
 */
UCLASS()
class GAME_API UMainUI_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindPlayerStatData(class UStatComponent_Game* PlayerStat);

	UInventoryUI_Game* GetInventoryWidget();
	UUIChatting_Game* GetChatting_Widget();

	void UpdateAll();
	void UpdatePartyUI();

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UCanvasPanel* CanvasPanel_Main;

protected:
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;

	void Init();
	void UpdatePlayerHPBar();
	void UpdatePlayerExpBar();
	void UpdateLevelText();
	void UpdatePlayerImage();

	UPROPERTY(meta=(BindWidget))
	UInventoryUI_Game* Inventory_Widget;

	UPROPERTY()
	TArray<class UPartyPlayerUI_Game*> Player_Widgets;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_Exp;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Exp;

	UPROPERTY(meta=(BindWidget))
	UImage* Image_Player;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Level;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_HP;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_HP;

	UPROPERTY(meta = (BindWidget))
	UUIChatting_Game* Chatting_Widget;
	
	TWeakObjectPtr<class UStatComponent_Game> CurrentPlayerStat;
	TWeakObjectPtr<class APlayer_Game> Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Party")
	TSubclassOf<UPartyPlayerUI_Game> PartyPlayerUIClass;
};
