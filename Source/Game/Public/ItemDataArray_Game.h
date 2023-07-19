// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "UObject/NoExportTypes.h"
#include "GameInstance_Game.h"
#include "ItemDataArray_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UItemDataArray_Game : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDataArray_Game();
	void Setting(const TArray<FItemData>& Array);
	
	TArray<FItemData> ItemDataArray;

	UPROPERTY()
	int32 ListNum;

	UPROPERTY()
	ESlotType Type;
};
