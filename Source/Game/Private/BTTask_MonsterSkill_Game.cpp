// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterSkill_Game.h"
#include "AIController_Game.h"
#include "Monster_Game.h"
#include "AnimInstance_Game.h"

UBTTask_MonsterSkill_Game::UBTTask_MonsterSkill_Game()
{
	bNotifyTick = true;
	NodeName = TEXT("SkillAttack");
	SkillNumber = -1;
	IsUsingSkill = false;
}

EBTNodeResult::Type UBTTask_MonsterSkill_Game::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Monster = Cast<AMonster_Game>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Monster->MulitcastSkill(SkillNumber);
	IsUsingSkill = true;
	Monster->AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_MonsterSkill_Game::OnSkillMontageEnded);

	return EBTNodeResult::InProgress;
}

void UBTTask_MonsterSkill_Game::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsUsingSkill)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_MonsterSkill_Game::OnSkillMontageEnded_Implementation(UAnimMontage* Montage, bool bInterrupted)
{
	if (Monster->AnimInstance->GetSkillMontage(SkillNumber) == Montage)
	{
		IsUsingSkill = false;
	}
}
