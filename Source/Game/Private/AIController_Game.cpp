// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Game.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AAIController_Game::HomePosKey(TEXT("HomePosKey"));
const FName AAIController_Game::PatrolPosKey(TEXT("PatrolPosKey"));
const FName AAIController_Game::TargetKey(TEXT("TargetKey"));
const FName AAIController_Game::SkillNumberKey(TEXT("SkillNumberKey"));

void AAIController_Game::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BBAsset = BTAsset->GetBlackboardAsset();
	check((IsValid(BBAsset) == true) && (IsValid(BTAsset) == true));

	RestartAI();
}

void AAIController_Game::RestartAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
	}
}

void AAIController_Game::StopAI()
{
	auto behaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorTreeComponent == nullptr)
	{
		return;
	}

	ResetTarget();
	behaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void AAIController_Game::ResetTarget()
{
	Blackboard->SetValueAsObject(TargetKey, nullptr);
}
