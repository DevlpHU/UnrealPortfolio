// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "GameFramework/Actor.h"
#include "DamageRenderActor_Game.generated.h"

UCLASS()
class GAME_API ADamageRenderActor_Game : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageRenderActor_Game();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamageText(float Damage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageText")
	class UTextRenderComponent* TextRender;

};
