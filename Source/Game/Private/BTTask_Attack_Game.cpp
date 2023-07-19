// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack_Game.h"
#include "AIController_Game.h"
#include "Monster_Game.h"
#include "AnimInstance_Game.h"

UBTTask_Attack_Game::UBTTask_Attack_Game()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack_Game::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Monster = Cast<AMonster_Game>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;

	Monster->MulitcastAttack();
	IsAttacking = true;
	Monster->AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_Attack_Game::OnAttackMontageEnded);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack_Game::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Attack_Game::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Monster->AnimInstance->GetAttackMontage() == Montage)
	{
		IsAttacking = false;
		if (Monster->CurMonsterState < ECurCharacterState::DeadStart)
		{
			Monster->ServerSetMonsterState(ECurCharacterState::Idle);
		}
	}
}
