// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Game.h"
#include "MainUI_Game.h"
#include "InventoryUI_Game.h"
#include "GameInstance_Game.h"
#include "UIChatting_Game.h"
#include "Player_Game.h"
#include "StatComponent_Game.h"
#include "UIAskParty_Game.h"
#include "Components/CanvasPanel.h"
#include "MoviePlayer.h"

APlayerController_Game::APlayerController_Game()
{
	static ConstructorHelpers::FClassFinder<UMainUI_Game> UI_HUD_C(TEXT("/Game/Blueprints/Widget/Main_Widget.Main_Widget_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUIAskParty_Game> UI_PARTY_C(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/AskParty_Widget.AskParty_Widget_C'"));
	if (UI_PARTY_C.Succeeded())
	{
		PartyWidgetClass = UI_PARTY_C.Class;
	}

	GameInstance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
	InteractedNPC = nullptr;

	bReplicates = true;
}

void APlayerController_Game::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void APlayerController_Game::SetInputGameModes(bool IsGameMode, bool IsUIMode, bool IsShowCursor)
{
	if (IsGameMode == true)
	{
		if (IsUIMode == true)
		{
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
		}
		else
		{
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
		}
	}
	else
	{
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
	}

	SetShowMouseCursor(IsShowCursor);
}

UMainUI_Game* APlayerController_Game::GetHUDWidget() const
{
	return HUDWidget;
}

TArray<FItemData>& APlayerController_Game::GetInventory()
{
	return Inventory;
}

void APlayerController_Game::PutItemToInventory(FItemData ItemData)
{
	if (ItemData.BaseData.Type == EItemType::Potion)
	{
		if (ItemList.Contains(ItemData.BaseData.Name))
		{
			TArray<int32> lists;
			ItemList.MultiFind(ItemData.BaseData.Name, lists, true);
			for (int32 slotNum : lists)
			{
				if (Inventory[slotNum].Count + ItemData.Count <= 99)
				{
					Inventory[slotNum].Count += ItemData.Count;
					ItemData.Count = 0;
					break;
				}
				else 
				{
					if (Inventory[slotNum].Count <= 99)
					{
						ItemData.Count = Inventory[slotNum].Count + ItemData.Count - 99;
						Inventory[slotNum].Count = 99;
					}
				}
			}

		}
	}

	if (ItemData.Count > 0) 
	{
		ItemList.Emplace(ItemData.BaseData.Name, Inventory.Num());
		Inventory.Emplace(ItemData);
	}

	OnInventoryDele.ExecuteIfBound();
}

void APlayerController_Game::OrganizeInventory()
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].Count <= 0)
		{
			Inventory.RemoveAt(i);
		}
	}

	ItemList.Empty();

	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		ItemList.Emplace(Inventory[i].BaseData.Name, i);
	}
}

void APlayerController_Game::ServerDestroy_Implementation(AActor* Actor)
{
	Actor->Destroy();
}


void APlayerController_Game::ServerSendMessage_Implementation(const FText& Text)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		APlayerController_Game* controller = Cast<APlayerController_Game>(OutActor);
		if (controller)
		{
			controller->ClientUpdateChatting(Text);
		}
	}
}

void APlayerController_Game::ClientUpdateChatting_Implementation(const FText& Text)
{
	HUDWidget->GetChatting_Widget()->UpdateChatting(Text);
}

void APlayerController_Game::ShowPartyUI(APlayer_Game* AskPlayer)
{
	if (PartyWidgetClass == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("PartyWidget Null"));
		return;
	}

	UUIAskParty_Game* widget = CreateWidget<UUIAskParty_Game>(this, PartyWidgetClass);
	if (widget == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("widget create failed"));
		return;
	}

	if (widget->SetAskPlayer(AskPlayer))
	{
		if (HUDWidget == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("HUD NULL"));
			return;
		}

		if (HUDWidget->CanvasPanel_Main == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("canvaspanel NULL"));
			return;
		}

		SetInputGameModes(true, true, true);
		widget->AddToViewport();
	}
}


void APlayerController_Game::ServerSetIsReady_Implementation(bool Ready)
{
	IsReady = Ready;
}

void APlayerController_Game::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false && GetPawn() != nullptr)
	{
		PlayerCharacter = Cast<APlayer_Game>(GetPawn());
		
		if (PlayerCharacter.IsValid())
		{
			SetInputGameModes();

			HUDWidget = CreateWidget<UMainUI_Game>(this, HUDWidgetClass);
			HUDWidget->AddToPlayerScreen();

			OnInventoryDele.BindLambda([this]()->void {
				Cast<UInventoryUI_Game>(HUDWidget->GetInventoryWidget())->Refresh();
				});

			HUDWidget->BindPlayerStatData(PlayerCharacter->PlayerStat);
			HUDWidget->UpdateAll();

			GameInstance->LoadInventory(this);
		}
	}
}

void APlayerController_Game::EndPlay(EEndPlayReason::Type EndType)
{
	Super::EndPlay(EndType);
}

void APlayerController_Game::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	GameInstance->SaveInventory(this);

	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
}
