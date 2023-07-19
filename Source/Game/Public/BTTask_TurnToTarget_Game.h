// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTTask_TurnToTarget_Game : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnToTarget_Game();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
	float TurnSpeed;
};
