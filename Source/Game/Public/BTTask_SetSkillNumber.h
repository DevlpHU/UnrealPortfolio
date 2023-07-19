// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetSkillNumber.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTTask_SetSkillNumber : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SetSkillNumber();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
