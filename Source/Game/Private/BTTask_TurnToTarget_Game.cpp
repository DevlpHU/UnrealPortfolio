// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget_Game.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player_Game.h"
#include "AIController_Game.h"
#include "Monster_Game.h"

UBTTask_TurnToTarget_Game::UBTTask_TurnToTarget_Game()
{
	NodeName = TEXT("TurnToTarget");
	TurnSpeed = 2.0f;
}

EBTNodeResult::Type UBTTask_TurnToTarget_Game::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto monster = Cast<AMonster_Game>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == monster)
		return EBTNodeResult::Failed;

	auto target = Cast<APlayer_Game>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIController_Game::TargetKey));
	if (nullptr == target)
		return EBTNodeResult::Failed;

	FVector lookVector = target->GetActorLocation() - monster->GetActorLocation();
	lookVector.Z = 0.0f;

	FRotator targetRot = FRotationMatrix::MakeFromX(lookVector).Rotator();
	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), targetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
