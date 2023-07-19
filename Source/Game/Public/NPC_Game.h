// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "GameFramework/Pawn.h"
#include "NPC_Game.generated.h"

class AItem_Game;
UCLASS()
class GAME_API ANPC_Game : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPC_Game();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void OnInteracted(class APlayerController_Game* ClientController);
	
	TArray<TSubclassOf<AItem_Game> > GetItemList() { return ItemList; }
	void SetItemList(TArray<TSubclassOf<AItem_Game> >& Items);

	void SetIsInteraction(bool bInteraction) { IsInteraction = bInteraction; }

	void CreateUISetting();
	void RemoveUISetting();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> InteractionUIClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Name")
	FText NpcName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestNPC")
	FText QuestName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Name")
	class UWidgetComponent* Name;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UUserWidget* InteractionUI;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool IsInteraction;

	//상인 : 파는 아이템 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MerchantNPC")
	TArray<TSubclassOf<AItem_Game> > ItemList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Type")
	ENPCType Type;

	class APlayerController_Game* InteractingController;
};
