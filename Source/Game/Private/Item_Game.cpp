// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Game.h"
#include "GameInstance_Game.h"
#include "Player_Game.h"
#include "StatComponent_Game.h"

// Sets default values
AItem_Game::AItem_Game()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = Sphere;
	Mesh->SetupAttachment(Sphere);

	Sphere->SetCollisionProfileName(TEXT("Item"));
	Sphere->SetSphereRadius(100.0f);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AItem_Game::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_SimulatedProxy && !Name.IsNone()) 
	{
		Data.BaseData = (*Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemBaseData(Name));
	}
}

void AItem_Game::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Cast<APlayer_Game>(GetOwner()) == nullptr)
	{
		SetActorEnableCollision(true);
	}
	else
	{
		SetActorEnableCollision(false);
	}
}
