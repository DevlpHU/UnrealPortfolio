// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "UISlot_Game.generated.h"

class UButton;
class UImage;
class UTextBlock;
class USizeBox;
class UUIInformation_Game;

/**
 * 
 */
UCLASS()
class GAME_API UUISlot_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	UUISlot_Game(const FObjectInitializer& ObjectInitializer);

	void Refresh(struct FItemData& ItemData);
	void DefaultRefresh();
	void SetSlotType(ESlotType SlotType) { Type = SlotType; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number")
	int32 SlotNum;

	UPROPERTY()
	UUIInformation_Game* InforWidget;

	UPROPERTY()
	class UCanvasPanelSlot* MainPanelSlotInfor;

protected:
	virtual void SynchronizeProperties() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	void Init();

	UFUNCTION()
	void Use();

	UFUNCTION()
	void Buy();

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Slot;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Default;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Slot;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Slot;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Equip;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Information")
	TSubclassOf<UUIInformation_Game> InforWidgetClass;

	UPROPERTY()
	class APlayerController_Game* PlayerController;

	UPROPERTY(VisibleAnywhere, Category = "Slot")
	ESlotType Type;

};
