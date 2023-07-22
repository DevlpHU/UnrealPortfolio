// Fill out your copyright notice in the Description page of Project Settings.


#include "UIAskParty_Game.h"
#include "Components/Button.h"
#include "Player_Game.h"
#include "PlayerController_Game.h"

void UUIAskParty_Game::NativeConstruct()
{
	Button_Accept->OnClicked.AddDynamic(this, &UUIAskParty_Game::Accept);
	Button_Refuse->OnClicked.AddDynamic(this, &UUIAskParty_Game::Refuse);
}

void UUIAskParty_Game::ControllerInputInit()
{
	APlayerController_Game* controller = Cast<APlayerController_Game>(GetOwningPlayer());
	if (controller == nullptr)
	{
		return;
	}

	controller->SetInputGameModes(true, false, false);
}

void UUIAskParty_Game::Accept()
{
	if (AskPlayer == nullptr)
	{
		return;
	}

	Cast<APlayer_Game>(GetOwningPlayerPawn())->AcceptParty(AskPlayer);
	
	this->RemoveFromParent();
	ControllerInputInit();
}

void UUIAskParty_Game::Refuse()
{
	if (AskPlayer == nullptr)
	{
		return;
	}

	this->RemoveFromParent();
	ControllerInputInit();
}

bool UUIAskParty_Game::SetAskPlayer(APlayer_Game* AskPlyr)
{
	if (AskPlyr == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("widget Askplayer Null"));
		return false;
	}

	AskPlayer = AskPlyr;
	return true;
}
