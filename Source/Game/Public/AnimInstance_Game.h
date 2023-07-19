// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Game.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate);
DECLARE_DELEGATE_OneParam(FCharStateChangedDelegate, ECurCharacterState);
DECLARE_DELEGATE(FJumpEndedDelegate);

/**
 * 
 */
UCLASS()
class GAME_API UAnimInstance_Game : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstance_Game();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlaySkillMontage(int32 Num);
	void PlayPickingMontage();
	void PlayDeadMontage();

	UAnimMontage* GetAttackMontage() { return AttackMontage; }
	UAnimMontage* GetPickingMontage() { return PickingMontage; }
	UAnimMontage* GetSkillMontage(int32 Num) { return SkillMontageArray[Num]; }
	UAnimMontage* GetDeadMontage() { return DeadMontage; }

	FOnAttackCheckDelegate OnAttackCheckDele;
	FCharStateChangedDelegate CharStateChangedDele;
	FJumpEndedDelegate	JumpEndedDele;

private:
	UFUNCTION()
	void AnimNotify_OnAttackCheck();

	UFUNCTION()
	void AnimNotify_OnChargeTimer();
	
	UFUNCTION()
	void AnimNotify_JumpEnded();

	UFUNCTION()
	void AnimNotify_OnDeadEnd();
	
	UFUNCTION(BlueprintCallable)
	virtual void EndTimerFunction();

	void SetTime();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = true))
	float Time;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> SkillMontageArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* PickingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;

	UPROPERTY(BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = true))
	int32 CurrentSkillIndex;
};
