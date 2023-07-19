// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "GameFramework/Actor.h"
#include "GameInstance_Game.h"
#include "Item_Game.generated.h"

UCLASS()
class GAME_API AItem_Game : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Game();

	UPROPERTY(VisibleAnywhere, Category = "Item")
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName Name;

	UPROPERTY()
	FItemData Data;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
