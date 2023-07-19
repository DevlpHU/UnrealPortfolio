// Fill out your copyright notice in the Description page of Project Settings.


#include "UISlot_Game.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "PlayerController_Game.h"
#include "GameInstance_Game.h"
#include "Item_Game.h"
#include "Player_Game.h"
#include "UIInformation_Game.h"
#include "MainUI_Game.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UIStore_Game.h"
#include "InventoryUISlots_Game.h"
#include "NPC_Game.h"

UUISlot_Game::UUISlot_Game(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUIInformation_Game> UI_INF_C(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Information_Widget.Information_Widget_C'"));
	if (UI_INF_C.Succeeded())	InforWidgetClass = UI_INF_C.Class;

	Type = ESlotType::None;
}

void UUISlot_Game::NativeConstruct()
{
	
}

void UUISlot_Game::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	Init();
}

void UUISlot_Game::NativeOnInitialized()
{
	Init();

	PlayerController = Cast<APlayerController_Game>(GetOwningPlayer());
	InforWidget = CreateWidget<UUIInformation_Game>(PlayerController, InforWidgetClass);

	InforWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUISlot_Game::Init()
{
	SizeBox_Slot->SetWidthOverride(100.0f);
	SizeBox_Slot->SetHeightOverride(100.0f);

	Image_Default->Brush.ImageSize =FVector2D (100.0f, 100.0f);

	Button_Slot->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::NoDrawType;
	
	Image_Slot->Brush.ImageSize = FVector2D(85.0f, 85.0f);
	Image_Slot->Brush.DrawAs = ESlateBrushDrawType::NoDrawType;

	Text_Equip->Font.Size = 20;
	Text_Equip->SetText(FText::FromString("E"));
	Text_Equip->SetVisibility(ESlateVisibility::Collapsed);

	Text_Count->Font.Size = 20;
	Text_Count->SetVisibility(ESlateVisibility::Collapsed);

	SlotNum = -1;
}

void UUISlot_Game::Use()
{
	if (IsValid(PlayerController))
	{
		auto player = Cast<APlayer_Game>(PlayerController->GetPawn());
		if (IsValid(player) && (SlotNum != -1))
		{
			PlayerController->GetInventory()[SlotNum].Use(*player, SlotNum);
		}
	}

}

void UUISlot_Game::Buy()
{
	if (IsValid(PlayerController))
	{
		auto npc = PlayerController->GetInteractedNPC();
		auto instance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));

		PlayerController->PutItemToInventory(FItemData(*(instance->GetItemBaseData(Cast<AItem_Game>(npc->GetItemList()[SlotNum].GetDefaultObject())->Name))));
	}
}

void UUISlot_Game::Refresh(FItemData& ItemData)
{
	Image_Slot->Brush.SetResourceObject(ItemData.BaseData.Icon);
	Image_Slot->Brush.DrawAs = ESlateBrushDrawType::Image;

	Text_Count->SetText(FText::FromString(FString::FromInt(ItemData.Count)));
	if (ItemData.Count > 1)	Text_Count->SetVisibility(ESlateVisibility::Visible);
	else					Text_Count->SetVisibility(ESlateVisibility::Collapsed);

	if (ItemData.IsEquip)	Text_Equip->SetVisibility(ESlateVisibility::Visible);
	else					Text_Equip->SetVisibility(ESlateVisibility::Collapsed);
}

void UUISlot_Game::DefaultRefresh()
{
	Image_Slot->Brush.DrawAs = ESlateBrushDrawType::NoDrawType;
	Text_Count->SetVisibility(ESlateVisibility::Collapsed);
	Text_Equip->SetVisibility(ESlateVisibility::Collapsed);

	SlotNum = -1;
}

FReply UUISlot_Game::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		switch (Type)
		{
		case ESlotType::Inventory:
		{
			Use();
		}
		break;
		case ESlotType::Store:
		{
			Buy();
		}
		break;
		}
	}

	return reply.NativeReply;
}

FReply UUISlot_Game::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	
	if (SlotNum == -1) return reply.NativeReply;

	if (IsValid(MainPanelSlotInfor)) {
		MainPanelSlotInfor->SetPosition(InMouseEvent.GetScreenSpacePosition());
	}

	return reply.NativeReply;
}

void UUISlot_Game::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (SlotNum == -1) return;

	MainPanelSlotInfor = Cast<UCanvasPanelSlot>(PlayerController->GetHUDWidget()->CanvasPanel_Main->AddChild(InforWidget));

	if (IsValid(MainPanelSlotInfor)) {
		switch (Type)
		{
		case ESlotType::Inventory:
		{
			InforWidget->Setting(PlayerController->GetInventory()[SlotNum].BaseData);
		}
		break;
		case ESlotType::Store:
		case ESlotType::Quest:
		{
			auto npc = PlayerController->GetInteractedNPC();
			auto instance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
			InforWidget->Setting(*(instance->GetItemBaseData(Cast<AItem_Game>(npc->GetItemList()[SlotNum].GetDefaultObject())->Name)));
		}
		break;
		}
		MainPanelSlotInfor->SetSize(FVector2D(200.0f, 200.0f));
	}
}

void UUISlot_Game::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (SlotNum == -1) return;

	InforWidget->RemoveFromParent();
}
