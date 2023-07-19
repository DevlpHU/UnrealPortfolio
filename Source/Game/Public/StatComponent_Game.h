// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Components/ActorComponent.h"
#include "StatComponent_Game.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_DELEGATE(FOnHPChangedDele);
DECLARE_DELEGATE(FOnExpChangedDele);
DECLARE_DELEGATE(FOnLevelUpDele);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UStatComponent_Game : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent_Game();
	
	UFUNCTION(BlueprintCallable)
	void SetNewLevel(int32 NewLevel);

	void SetDamage(float NewDamage, AController* EventInstigator);
	void SetStatDataType(EStatComponentType Type) { StatType = Type; }
	void SetEquipAttack(float Attack);
	void SetEquipHP(float HP);
	void AddCurrentExp(int32 AddExp);

	bool IsStatDataValid() { return CurrentPlayerStatData != nullptr || CurrentMonsterStatData != nullptr; }

	int32 GetLevel() { return Level; }
	float GetCurrentHP() { return CurrentHP; }
	float GetTotalHP() { return TotalHP; }
	float GetEquipHP() { return EquipHP; }

	UFUNCTION(BlueprintCallable)
	float GetHPRatio() { return CurrentHP / TotalHP; }

	float GetEquipAttack() { return EquipAttack; }
	float GetTotalAttack() { return TotalAttack; }
	int32 GetCurrentExp() { return CurrentExp; }
	int32 GetExp() { return Exp; }
	float GetExpRato() { return static_cast<float>(CurrentExp) / static_cast<float>(Exp); }

	void Revive();

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDele OnHPChanged;
	FOnExpChangedDele OnExpChanged;
	FOnLevelUpDele OnLevelUpDele;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Client, Reliable)
	void ClientOnHPChanged();

	UFUNCTION(Client, Reliable)
	void ClientOnExpChanged();

	UFUNCTION(Client, Reliable)
	void ClientOnLevelUp();

	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION()
	void OnRep_CurrentExp();

	UFUNCTION()
	void OnRep_Level();

private:
	//Level
	UPROPERTY(ReplicatedUsing = OnRep_Level, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 Level;

	//HP
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Replicated, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float EquipHP;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float TotalHP;

	//Atk
	UPROPERTY(Replicated, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float EquipAttack;

	UPROPERTY(Replicated, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float TotalAttack;

	//Player : NextExp , Monster : DropExp
	UPROPERTY(Replicated, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 Exp;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentExp, Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 CurrentExp;

	EStatComponentType StatType;
	struct FPlayerData* CurrentPlayerStatData = nullptr;
	struct FMonsterData* CurrentMonsterStatData = nullptr;
};
