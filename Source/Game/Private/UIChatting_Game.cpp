// Fill out your copyright notice in the Description page of Project Settings.


#include "UIChatting_Game.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerState_Game.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player_Game.h"
#include "PlayerController_Game.h"

void UUIChatting_Game::UpdateChatting(FText Chat)
{
	UTextBlock* chatBlock = NewObject<UTextBlock>(ScrollBox_Chat);
	chatBlock->SetText(Chat);
	chatBlock->Font.Size = 15;
	chatBlock->ColorAndOpacity = FSlateColor(FLinearColor(1.0f, 1.0f, 0.0f));

	ScrollBox_Chat->AddChild(chatBlock);
	ScrollBox_Chat->ScrollToEnd();
	EditableText_Chat->SetText(FText::GetEmpty());
}

void UUIChatting_Game::OnChat()
{
	EditableText_Chat->SetFocus();
}

void UUIChatting_Game::SendBtnClick()
{
	APlayerController_Game* controller = Cast<APlayerController_Game>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(controller) == false)
	{
		return;
	}

	FText txt = ConfigureText(EditableText_Chat->GetText());
	controller->ServerSendMessage(txt);
}

void UUIChatting_Game::NativeConstruct()
{
	Super::NativeConstruct();

	EditableText_Chat->OnTextCommitted.AddDynamic(this, &UUIChatting_Game::Committed);
	Button_Send->OnClicked.AddDynamic(this, &UUIChatting_Game::SendBtnClick);
}

FText UUIChatting_Game::ConfigureText(const FText& Msg)
{
	FText name = Cast<APlayer_Game>(GetOwningPlayerPawn())->GetPlayerName();
	return FText::FromString(UKismetStringLibrary::BuildString_Name(name.ToString(), FString(" : "), FName(*(Msg.ToString())), FString()));
}

void UUIChatting_Game::Committed(const FText& Text, ETextCommit::Type CommitMethod)
{
	APlayerController_Game* controller = Cast<APlayerController_Game>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(controller) == false)
	{
		return;
	}

	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			if (Text.IsEmpty() == false)
			{
				FText txt = ConfigureText(Text);
				controller->ServerSendMessage(txt);
			}
			break;
		}
		case ETextCommit::OnUserMovedFocus:
		{
			if (Text.IsEmpty() == false)
			{
				EditableText_Chat->SetText(FText::GetEmpty());
			}
			break;
		}
		default:
		{
			break;
		}
	}

	controller->SetInputGameModes(true, false, false);
}
