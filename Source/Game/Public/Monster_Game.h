// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "GameFramework/Character.h"
#include "Monster_Game.generated.h"


UCLASS()
class GAME_API AMonster_Game : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster_Game();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDead();

	UFUNCTION(NetMulticast, Reliable)
	void MulitcastAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulitcastSkill(int32 Num);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMonsterState(ECurCharacterState State);

	UFUNCTION()
	void SetState();

	UFUNCTION()
	void OnRep_State();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	class UStatComponent_Game* MonsterStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Name")
	FName Name;

	UPROPERTY()
	class UAnimInstance_Game* AnimInstance;

	UPROPERTY(ReplicatedUsing = OnRep_State, BlueprintReadWrite, EditAnywhere, Category = "State")
	ECurCharacterState CurMonsterState = ECurCharacterState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageRenderClass")
	TSubclassOf<class ADamageRenderActor_Game> DamageActorClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void SpawnDamageText(float Damage);

	void _AttackCheck();
	void _StateToRevive();

	FTimerHandle RespawnTimerHandle;
};
