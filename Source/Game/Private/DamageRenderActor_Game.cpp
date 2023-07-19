// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageRenderActor_Game.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ADamageRenderActor_Game::ADamageRenderActor_Game()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SCENE"));
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DAMAGETEXT"));

	RootComponent = Scene;
	TextRender->SetupAttachment(Scene);
}

void ADamageRenderActor_Game::SetDamageText(float Damage)
{
	TextRender->SetText(FText::FromString(FString::FromInt(Damage)));
}

// Called when the game starts or when spawned
void ADamageRenderActor_Game::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADamageRenderActor_Game::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

