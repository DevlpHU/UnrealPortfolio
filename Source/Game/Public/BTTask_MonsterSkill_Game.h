// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MonsterSkill_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTTask_MonsterSkill_Game : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MonsterSkill_Game();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION(NetMulticast, Reliable)
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SkillNumber;

private:
	bool IsUsingSkill = false;
	class AMonster_Game* Monster;
};
