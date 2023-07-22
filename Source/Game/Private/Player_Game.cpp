// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Game.h"
#include "AnimInstance_Game.h"
#include "Monster_Game.h"
#include "StatComponent_Game.h"
#include "Blueprint/UserWidget.h"
#include "Item_Game.h"
#include "NPC_Game.h"
#include "PlayerController_Game.h"
#include "MainUI_Game.h"
#include "InventoryUI_Game.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MainUI_Game.h"
#include "UIChatting_Game.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "GameInstance_Game.h"
#include "PlayerState_Game.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
APlayer_Game::APlayer_Game()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm	= CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera		= CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	PlayerStat	= CreateDefaultSubobject<UStatComponent_Game>(TEXT("PLAYERSTAT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PLAYER(TEXT("/Game/Character/Player_Sword/Player_Sword.Player_Sword"));
	if (SK_PLAYER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_PLAYER.Object);
	}

	GetCapsuleComponent()->SetCollisionProfileName(FName("CharacterMesh"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Blueprints/Anim/AnimBP_PlayerSword.AnimBP_PlayerSword_C"));
	if (PLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	_CurPlayerState = ECurCharacterState::Idle;
	PlayerStat->SetIsReplicated(true);
	PlayerStat->SetStatDataType(EStatComponentType::Player);

	WeaponSokcetName = FName("hand_rSocket");
	CurEquipWeaponSlot = -1;

	PlayerName = FText::GetEmpty();
	bReplicates = true;

	Party.Init(nullptr, MaxPartyNumber);
}


void APlayer_Game::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

// Called when the game starts or when spawned
void APlayer_Game::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority() == false)
	{
		UGameInstance_Game* GI = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
		ServerSetPlayerName(GI->Name);
	}
}

void APlayer_Game::CheckForwardPlayer()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * ScanDistance;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_GameTraceChannel3,
		Params
	);

	if (bResult)
	{
		ForwardPlayer = Cast<APlayer_Game>(HitResult.Actor);
		if (ForwardPlayer != nullptr)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f);
			return;
		}
	}
	else
	{
		ForwardPlayer = nullptr;
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f);
}

void APlayer_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayer_Game, Party);
	DOREPLIFETIME(APlayer_Game, MaxPartyNumber);
	DOREPLIFETIME(APlayer_Game, CurPlayerCount);
	DOREPLIFETIME(APlayer_Game, PlayerName);
}

// Called every frame
void APlayer_Game::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckForwardPlayer();
}

void APlayer_Game::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UAnimInstance_Game>(GetMesh()->GetAnimInstance());

	AnimInstance->OnMontageEnded.AddDynamic(this, &APlayer_Game::_OnAttackMontageEnded);
	AnimInstance->OnMontageEnded.AddDynamic(this, &APlayer_Game::_OnPickUpMontageEnded);
	AnimInstance->OnAttackCheckDele.AddUObject(this, &APlayer_Game::_AttackCheck);
	AnimInstance->JumpEndedDele.BindUObject(this, &APlayer_Game::_JumpEnded);
}

float APlayer_Game::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	PlayerStat->SetDamage(DamageAmount, EventInstigator);

	return DamageAmount;
}

// Called to bind functionality to input
void APlayer_Game::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayer_Game::_Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayer_Game::_Attack);
	PlayerInputComponent->BindAction(TEXT("GameInteraction"), EInputEvent::IE_Pressed, this, &APlayer_Game::_GameOnInteract);
	PlayerInputComponent->BindAction(TEXT("PlayerInteraction"), EInputEvent::IE_Pressed, this, &APlayer_Game::_PlayerOnInteract);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayer_Game::_Inventory);
	PlayerInputComponent->BindAction(TEXT("Chatting"), EInputEvent::IE_Pressed, this, &APlayer_Game::_Chat);

	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayer_Game::_MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayer_Game::_MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayer_Game::_Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayer_Game::_LookUp);
}

void APlayer_Game::EquipWeapon(TSubclassOf<class AItem_Game> Weapon)
{
	UnEquipWeapon();

	auto CurWeapon = GetWorld()->SpawnActor<AItem_Game>(Weapon, FVector::ZeroVector, FRotator::ZeroRotator);
	if (IsValid(CurWeapon))
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSokcetName);
		CurWeapon->SetOwner(this);
		CurEquipWeapon = CurWeapon;
	}
}

void APlayer_Game::UnEquipWeapon()
{
	if (CurEquipWeapon)
	{
		CurEquipWeapon->Destroy();
	}
	CurEquipWeapon = nullptr;
}

void APlayer_Game::ServerAskParty_Implementation(APlayer_Game* AskedPlayer)
{
	if (CurPlayerCount > MaxPartyNumber || AskedPlayer->CurPlayerCount > AskedPlayer->MaxPartyNumber)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("It's Full Party!"));
		return;
	}

	AskedPlayer->ClientAskedParty(this);
}

void APlayer_Game::ClientAskedParty_Implementation(APlayer_Game* AskPlayer)
{
	if (AskPlayer == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("AskPlayer Null"));
		return;
	}

	APlayerController_Game* MyController = Cast<APlayerController_Game>(Controller);
	if (MyController == nullptr)
	{
		return;
	}

	MyController->ShowPartyUI(AskPlayer);
}

void APlayer_Game::ServerSetEquipAttack_Implementation(float Attack)
{
	PlayerStat->SetEquipAttack(Attack);
}

void APlayer_Game::ServerSetEquipHP_Implementation(float HP)
{
	PlayerStat->SetEquipHP(HP);
}

void APlayer_Game::ServerSetDamage_Implementation(float HP)
{
	PlayerStat->SetDamage(HP, Controller);
}

//this : AskedPlayer
void APlayer_Game::ServerConfigureParty_Implementation(APlayer_Game* AskPlayer)
{
	for (int32 i = 0; i < AskPlayer->CurPlayerCount; ++i)
	{
		AskPlayer->Party[i]->PutPartyPlayer(this);
	}

	this->PutPartyPlayer(AskPlayer);
	for (int32 i = 0; i < AskPlayer->CurPlayerCount; ++i)
	{
		this->PutPartyPlayer(AskPlayer->Party[i]);
	}

	AskPlayer->PutPartyPlayer(this);
} 

void APlayer_Game::OnRep_Party()
{
	OnChangePartyPlayerDele.ExecuteIfBound();
}

void APlayer_Game::ServerSetPlayerName_Implementation(const FText& Name)
{
	PlayerName = Name;
}

void APlayer_Game::AcceptParty(APlayer_Game* AskPlayer)
{
	if (AskPlayer == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Accept Player Null"));
		return;
	}
	
	ServerConfigureParty(AskPlayer);
}

bool APlayer_Game::IsInParty(APlayer_Game* AskedPlayer)
{
	for (APlayer_Game* PartyPlayer : Party)
	{
		if (PartyPlayer == AskedPlayer)
		{
			return true;
		}
	}
	return false;
}

void APlayer_Game::PutPartyPlayer(APlayer_Game* PartyPlayer)
{
	PartyAccessGuard.Lock();
	for (int32 i = 0; i < MaxPartyNumber; i++)
	{
		if (Party[i] == nullptr)
		{
			Party[i] = PartyPlayer;
			break;
		}
	}
	CurPlayerCount++;
	PartyAccessGuard.Unlock();
}

void APlayer_Game::ServerEquipWeapon_Implementation(TSubclassOf<class AItem_Game> Weapon)
{
	EquipWeapon(Weapon);
}

void APlayer_Game::ServerUnEquipWeapon_Implementation()
{
	UnEquipWeapon();
}

void APlayer_Game::_MoveRight(float NewAxisValue)
{
	if (_CurPlayerState != ECurCharacterState::Idle)
	{
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void APlayer_Game::_MoveForward(float NewAxisValue)
{
	if (_CurPlayerState != ECurCharacterState::Idle) 
	{
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void APlayer_Game::_Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void APlayer_Game::_LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void APlayer_Game::_Attack()
{
	if (!IsValid(AnimInstance) || _CurPlayerState != ECurCharacterState::Idle)
	{
		return;
	}

	ServerPlayAttackMontage();
	_CurPlayerState = ECurCharacterState::Attacking;
}

void APlayer_Game::_Jump()
{
	if (!IsValid(AnimInstance) || _CurPlayerState != ECurCharacterState::Idle)
	{
		return;
	}

	Super::Jump();
	_CurPlayerState = ECurCharacterState::Jumping;
}

void APlayer_Game::_PlayerOnInteract()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(CurPlayerCount));
	if (ForwardPlayer == nullptr)
	{
		return;
	}
	else if (IsInParty(ForwardPlayer))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("It's Party Player"));
		return;
	}
	else if (ForwardPlayer->CurPlayerCount > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("He has Party"));
		return;
	}
	else if ((CurPlayerCount + 1) + (ForwardPlayer->CurPlayerCount + 1) > MaxPartyNumber)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("It's PartyNum Over"));
		return;
	}

	ServerAskParty(ForwardPlayer);
}

void APlayer_Game::_GameOnInteract()
{
	if (_CurPlayerState != ECurCharacterState::Idle)
	{
		return;
	}
	_CurPlayerState = ECurCharacterState::Interacting;

	TArray<FHitResult> CheckResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
		CheckResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(100.0f),
		Params
	);

	if (bResult)
	{
		for (auto CheckResult : CheckResults)
		{
			auto CheckItem = Cast<AItem_Game>(CheckResult.Actor);
			auto CheckNPC = Cast<ANPC_Game>(CheckResult.Actor);
			auto playerController = Cast<APlayerController_Game>(GetController());

			if (IsValid(playerController))
			{
				if (IsValid(CheckItem))
				{
					ServerPlayPickUpMontage();
					playerController->PutItemToInventory(CheckItem->Data);
					playerController->ServerDestroy(CheckItem);
				}

				if (IsValid(CheckNPC))
				{
					CheckNPC->OnInteracted(playerController);
					_CurPlayerState = ECurCharacterState::Idle;
					break;
				}
			}
		}
	}
	else
	{
		_CurPlayerState = ECurCharacterState::Idle;
	}
}

void APlayer_Game::_Inventory()
{
	auto playerController = Cast<APlayerController_Game>(GetController());
	auto InventoryWidget = playerController->GetHUDWidget()->GetInventoryWidget();
	
	switch (InventoryWidget->GetVisibility())
	{
		case ESlateVisibility::Collapsed:
		{
			InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			playerController->SetInputGameModes(true, true, true);
			break;
		}
		case ESlateVisibility::SelfHitTestInvisible:
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			playerController->SetInputGameModes(true, false, false);
			break;
		}
		default:
		{
			break;
		}
	}
}

void APlayer_Game::_Chat()
{
	auto playerController = Cast<APlayerController_Game>(Controller);
	if (playerController != nullptr)
	{
		playerController->SetInputGameModes(false, true, true);
		playerController->GetHUDWidget()->GetChatting_Widget()->OnChat();
	}
}

void APlayer_Game::_OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimInstance->GetAttackMontage())
	{
		_CurPlayerState = ECurCharacterState::Idle;
	}
}

void APlayer_Game::_OnPickUpMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimInstance->GetPickingMontage())
	{
		_CurPlayerState = ECurCharacterState::Idle;
	}
}

void APlayer_Game::_JumpEnded()
{
	if (_CurPlayerState == ECurCharacterState::Jumping) {}
	{
		_CurPlayerState = ECurCharacterState::Idle;
	}
}

void APlayer_Game::_AttackCheck()
{
	if (GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);
	
	if (bResult)
	{
		for (auto hitResult : HitResults)
		{
			auto hitActor = Cast<AMonster_Game>(hitResult.Actor);

			if (IsValid(hitActor))
			{
				FDamageEvent DamageEvent;
				hitActor->TakeDamage(PlayerStat->GetTotalAttack(), DamageEvent, GetController(), this);
			}
		}
	}
}

void APlayer_Game::ServerPlayPickUpMontage_Implementation()
{
	MulticastPlayMontage(AnimInstance->GetPickingMontage());
}

void APlayer_Game::ServerPlayAttackMontage_Implementation()
{
	MulticastPlayMontage(AnimInstance->GetAttackMontage());
}

void APlayer_Game::MulticastPlayMontage_Implementation(UAnimMontage* Montage)
{
	if (AnimInstance->Montage_IsPlaying(Montage) == false)
	{
		PlayAnimMontage(Montage, 1.f);
	}
}

