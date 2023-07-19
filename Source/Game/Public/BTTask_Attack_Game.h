// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTTask_Attack_Game : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack_Game();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	bool IsAttacking = false;
	class AMonster_Game* Monster;
};
