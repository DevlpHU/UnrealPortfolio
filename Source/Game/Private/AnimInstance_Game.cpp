// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Game.h"
#include "PlayerController_Game.h"
#include "Player_Game.h"

UAnimInstance_Game::UAnimInstance_Game()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
}

void UAnimInstance_Game::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)	
			IsInAir = Character->GetMovementComponent()->IsFalling();
	}
}

void UAnimInstance_Game::PlayAttackMontage()
{
	if (Montage_IsPlaying(AttackMontage) == false)
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UAnimInstance_Game::PlaySkillMontage(int32 Num)
{
	if (Montage_IsPlaying(SkillMontageArray[Num]) == false)
	{
		Montage_Play(SkillMontageArray[Num], 1.f);
		CurrentSkillIndex = Num;
	}
}

void UAnimInstance_Game::PlayPickingMontage()
{
	if (Montage_IsPlaying(PickingMontage) == false)
	{
		Montage_Play(PickingMontage, 1.f);
	}
}

void UAnimInstance_Game::PlayDeadMontage()
{
	if (Montage_IsPlaying(DeadMontage) == false)
	{
		Montage_Play(DeadMontage, 1.f);
	}
}

void UAnimInstance_Game::AnimNotify_OnAttackCheck()
{
	OnAttackCheckDele.Broadcast();
}

void UAnimInstance_Game::AnimNotify_OnChargeTimer()
{
	for (int32 i = 0; i < SkillMontageArray.Num(); ++i)
	{
		if (GetCurrentActiveMontage() == SkillMontageArray[i])
			CurrentSkillIndex = i;
	}

	if (CurrentSkillIndex == -1) return;
	
	SetTime();
	
	if (Montage_IsPlaying(SkillMontageArray[CurrentSkillIndex]))
	{
		FTimerDelegate TimerCallback;
		FTimerHandle ChargeTimer;
		TimerCallback.BindUFunction(this, FName("EndTimerFunction"));

		Montage_Pause(SkillMontageArray[CurrentSkillIndex]);
		TryGetPawnOwner()->GetWorldTimerManager().SetTimer(ChargeTimer, TimerCallback, Time, false);
	}
}

void UAnimInstance_Game::AnimNotify_JumpEnded()
{
	JumpEndedDele.Execute();
}

void UAnimInstance_Game::AnimNotify_OnDeadEnd()
{
	CharStateChangedDele.ExecuteIfBound(ECurCharacterState::DeadEnd);
}

void UAnimInstance_Game::EndTimerFunction()
{
	Montage_Resume(SkillMontageArray[CurrentSkillIndex]);
}

void UAnimInstance_Game::SetTime()
{
	switch (CurrentSkillIndex)
	{
	case 2:
	{	Time = 3.f;	}	break;
	case 3:
	{	Time = 7.f;	}	break;
	default:
	{	Time = -1.f;}	break;
	}
}
