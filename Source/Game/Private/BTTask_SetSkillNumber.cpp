// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetSkillNumber.h"
#include "AIController_Game.h"
#include "Monster_Game.h"
#include "StatComponent_Game.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetSkillNumber::UBTTask_SetSkillNumber()
{
	NodeName = TEXT("SetSkillNum");
}

EBTNodeResult::Type UBTTask_SetSkillNumber::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster_Game* monster = Cast<AMonster_Game>(OwnerComp.GetAIOwner()->GetPawn());
	if (monster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	int32 number;

	int32 HPRatio = (monster->MonsterStat->GetCurrentHP() / monster->MonsterStat->GetTotalHP()) * 100;
	if ((HPRatio < 20))
	{
		number = FMath::RandRange(0, 3);
	}
	else
	{
		number = FMath::RandRange(0, 2);
	}

	if (number < 0 || number > 3)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AAIController_Game::SkillNumberKey, number);
	return EBTNodeResult::Succeeded;
}