// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "AIController.h"
#include "AIController_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AAIController_Game : public AAIController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void RestartAI();

	UFUNCTION(BlueprintCallable)
	void StopAI();

	void ResetTarget();

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName SkillNumberKey;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BTAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBlackboardData* BBAsset;
};
