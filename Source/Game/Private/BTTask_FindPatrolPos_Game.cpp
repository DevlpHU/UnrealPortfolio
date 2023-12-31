// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos_Game.h"
#include "AIController_Game.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos_Game::UBTTask_FindPatrolPos_Game()
{
	NodeName = TEXT("FindPatrolPos");
	MaxPosDistace = 500.0f;
}

EBTNodeResult::Type UBTTask_FindPatrolPos_Game::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIController_Game::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, MaxPosDistace, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIController_Game::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}