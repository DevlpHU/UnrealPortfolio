// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange_Game.h"
#include "AIController_Game.h"
#include "Player_Game.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange_Game::UBTDecorator_IsInAttackRange_Game()
{
	NodeName = TEXT("CanAttack");
	Distance = 200.0f;
}

bool UBTDecorator_IsInAttackRange_Game::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == ControllingPawn)
	{
		return false;
	}

	auto Target = Cast<APlayer_Game>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIController_Game::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(ControllingPawn) <= Distance);
	return bResult;
}