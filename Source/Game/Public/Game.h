// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class EQuestProgressType : uint8
{
	Waiting,
	Proceeding,
	Complete,
	End
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	None,
	Killing
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	None,
	Inventory,
	Store,
	Quest
};

UENUM(BlueprintType)
enum class ENPCType : uint8
{
	None,
	Merchant,
	Quest
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Weapon,
	Potion
};

UENUM(BlueprintType)
enum class EStatComponentType : uint8
{
	Undefined,
	Player,
	Monster
};

UENUM(BlueprintType)
enum class ECurCharacterState : uint8
{
	Idle,
	Interacting,
	Jumping,
	Attacking,
	DeadStart, 
	DeadEnd,
	Revive
};
