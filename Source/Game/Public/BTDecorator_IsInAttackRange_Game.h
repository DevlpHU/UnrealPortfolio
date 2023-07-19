// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTDecorator_IsInAttackRange_Game : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInAttackRange_Game();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float Distance;
};
