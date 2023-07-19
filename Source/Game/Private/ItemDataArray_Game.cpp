// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataArray_Game.h"
#include "GameInstance_Game.h"

UItemDataArray_Game::UItemDataArray_Game()
{
	Type = ESlotType::None;
}


void UItemDataArray_Game::Setting(const TArray<FItemData>& Array)
{
	ListNum = 0;

	for (auto& item : Array)
	{
		ItemDataArray.Emplace(item);
	}
}

