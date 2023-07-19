// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTService_Detect_Game : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Detect_Game();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detect")
	float DetectRadius;
};
