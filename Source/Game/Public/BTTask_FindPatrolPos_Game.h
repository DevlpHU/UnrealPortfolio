// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTTask_FindPatrolPos_Game : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPos_Game();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPos")
	float MaxPosDistace;
};
