// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "GameInstance_Game.h"
#include "UIStore_Game.generated.h"

class UTextBlock;
class UImage;
class UListView;
/**
 * 
 */
UCLASS()
class GAME_API UUIStore_Game : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(TArray<TSubclassOf<AItem_Game> >& Items);

	void SetTextName(FText Name);
	void RegisterToStore();
	void SetTextSaleGold(int32 Gold);
	void SetDataList(TArray<TSubclassOf<AItem_Game> >& Items);

	FItemData GetItmeData(int32 Number) const { return DataList[Number]; }

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Store;

	UPROPERTY(meta = (BindWidget))
	UListView* ListView_StoreSlots;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Gold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TArray<FItemData> DataList;
};
