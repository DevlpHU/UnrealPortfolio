// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "GameFramework/Character.h"
#include "Player_Game.generated.h"

DECLARE_DELEGATE(FOnChangePartyPlayerDelegate);

UCLASS()
class GAME_API APlayer_Game : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Game();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//아이템 장착 함수 - 현재 착용 중인 아이템 존재 시 삭제 후 장착
	void EquipWeapon(TSubclassOf<class AItem_Game> Weapon);
	void UnEquipWeapon();

	//Party
	void AcceptParty(APlayer_Game* AskPlayer);
	bool IsInParty(APlayer_Game* AskedPlayer);
	void PutPartyPlayer(APlayer_Game* PartyPlayer);

	FText GetPlayerName() { return PlayerName; }

	//RPC
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(TSubclassOf<class AItem_Game> Weapon);

	UFUNCTION(Server, Reliable)
	void ServerUnEquipWeapon();

	UFUNCTION(Server, Reliable)
	void ServerAskParty(APlayer_Game* AskedPlayer);

	UFUNCTION(Client, Reliable)
	void ClientAskedParty(APlayer_Game* AskPlayer);

	UFUNCTION(Server, Reliable)
	void ServerSetEquipAttack(float Attack);

	UFUNCTION(Server, Reliable)
	void ServerSetEquipHP(float HP);

	UFUNCTION(Server, Reliable)
	void ServerSetDamage(float HP);

	UFUNCTION(Server, Reliable)
	void ServerConfigureParty(APlayer_Game* AskPlayer);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetPlayerName(const FText& Name);

	UFUNCTION()
	void OnRep_Party();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UStatComponent_Game* PlayerStat;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AItem_Game* CurEquipWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSokcetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 CurEquipWeaponSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Symbol")
	UTexture2D* Symbol;

	UPROPERTY(ReplicatedUsing = OnRep_Party, BlueprintReadOnly, Category = "Party")
	TArray<APlayer_Game*> Party;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Party")
	int32 MaxPartyNumber = 4;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Party")
	int32 CurPlayerCount = 0;

	FCriticalSection PartyAccessGuard;
	FOnChangePartyPlayerDelegate OnChangePartyPlayerDele;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckForwardPlayer();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	ECurCharacterState _CurPlayerState;

	UPROPERTY()
	class UAnimInstance_Game* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerScan")
	float ScanDistance = 250.f;

	UPROPERTY()
	APlayer_Game* ForwardPlayer;

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadWrite, Category = "Name")
	FText PlayerName;

private:
	void _MoveRight(float NewAxisValue);
	void _MoveForward(float NewAxisValue);
	void _Turn(float NewAxisValue);
	void _LookUp(float NewAxisValue);

	void _Attack();
	void _Jump();
	void _GameOnInteract();
	void _PlayerOnInteract();
	void _Inventory();
	void _Chat();

	UFUNCTION()
	void _OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void _OnPickUpMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void _JumpEnded();
	void _AttackCheck();

	//RPC
	UFUNCTION(Server, Reliable)
	void ServerPlayAttackMontage();

	UFUNCTION(Server, Reliable)
	void ServerPlayPickUpMontage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(UAnimMontage* Montage);
};
