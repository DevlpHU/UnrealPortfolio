// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect_Game.h"
#include "AIController_Game.h"
#include "Player_Game.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect_Game::UBTService_Detect_Game()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	DetectRadius = 600.0f;
}

void UBTService_Detect_Game::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float minDistance = DetectRadius;
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	if (nullptr == World) 
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	bool IsPlayer = false;
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			APlayer_Game* player = Cast<APlayer_Game>(OverlapResult.GetActor());

			if (player && player->GetController()->IsPlayerController())
			{
				auto dist = (player->GetActorLocation() - ControllingPawn->GetActorLocation()).Size();

				if (minDistance >= dist) 
				{
					BlackBoard->SetValueAsObject(AAIController_Game::TargetKey, player);
					minDistance = dist;
					IsPlayer = true;
				}
			}
		}
	}

	if (IsPlayer == false)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIController_Game::TargetKey, nullptr);
	}

	auto player = Cast<APlayer_Game>(BlackBoard->GetValueAsObject(AAIController_Game::TargetKey));
	
	DrawDebugSphere(World, Center, DetectRadius, 16, (player == nullptr) ? FColor::Red : FColor::Green, false, 0.2f);

	if (IsValid(player))
	{
		DrawDebugPoint(World, player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
		DrawDebugLine(World, ControllingPawn->GetActorLocation(), player->GetActorLocation(), FColor::Blue, false, 0.2f);
	}
}