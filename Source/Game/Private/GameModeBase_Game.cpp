// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase_Game.h"
#include "Player_Game.h"
#include "PlayerController_Game.h"
#include "PlayerState_Game.h"

AGameModeBase_Game::AGameModeBase_Game()
{
	static ConstructorHelpers::FClassFinder<APlayer_Game> PLAYER(TEXT("Blueprint'/Game/Blueprints/Player_BP.Player_BP_C'"));
	if (PLAYER.Succeeded())
	{
		DefaultPawnClass = PLAYER.Class;
	}

	PlayerControllerClass = APlayerController_Game::StaticClass();
	PlayerStateClass = APlayerState_Game::StaticClass();
}