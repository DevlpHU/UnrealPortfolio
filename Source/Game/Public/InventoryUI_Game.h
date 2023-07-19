// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI_Game.generated.h"

class UButton;
class UTextBlock;
class UListView;
/**
 * 
 */
UCLASS()
class GAME_API UInventoryUI_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	void Refresh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "List")
	int32 ListCount;

protected:
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;
	
	void Init();
	void SetGoldText(int32 Gold);

	UPROPERTY(meta=(BindWidget))
	UButton* Button_Inventory;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Inventory;

	UPROPERTY(meta=(BindWidget))
	UListView* ListView_Inventory;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Gold;
};
