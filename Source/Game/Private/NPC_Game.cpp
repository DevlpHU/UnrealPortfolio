// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Game.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/GameSession.h"
#include "Engine/Font.h"
#include "GameFramework/PlayerState.h"
#include "Player_Game.h"
#include "Components/WidgetComponent.h"
#include "Components/CanvasPanel.h"
#include "ActorWidget_Game.h"
#include "PlayerController_Game.h"
#include "MainUI_Game.h"
#include "Components/CanvasPanelSlot.h"
#include "UIStore_Game.h"
#include "UIQuest_Game.h"
#include "Item_Game.h"

// Sets default values
ANPC_Game::ANPC_Game()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Name = CreateDefaultSubobject<UWidgetComponent>(TEXT("NAME"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	Name->SetupAttachment(Mesh);

	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	Name->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	Name->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_NAME(TEXT("/Game/Blueprints/Widget/NPC_Widget.NPC_Widget_C"));
	if (UI_NAME.Succeeded())
	{
		Name->SetWidgetClass(UI_NAME.Class);
		Name->SetDrawSize(FVector2D(150.0f, 20.0f));
	}

	IsInteraction = false;
	Type = ENPCType::None;
}

void ANPC_Game::SetItemList(TArray<TSubclassOf<AItem_Game>>& Items)
{
	ItemList.Empty();
	for (auto item : Items)
	{
		ItemList.Emplace(item);
	}
}

// Called when the game starts or when spawned
void ANPC_Game::BeginPlay()
{
	Super::BeginPlay();
	
	if (Name->GetUserWidgetObject() == nullptr)
	{
		UUserWidget* nameWidget = CreateWidget(GetWorld(), Name->GetWidgetClass());
		Name->SetWidget(nameWidget);
	}

	Cast<UActorWidget_Game>(Name->GetUserWidgetObject())->SetTextNameText(NpcName);
}


// Called every frame
void ANPC_Game::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPC_Game::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Game::OnInteracted(APlayerController_Game* ClientController)
{
	auto instance = Cast<UGameInstance_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
	InteractingController = ClientController;

	//UI 오픈 및 방식 적용
	if (IsInteraction == false)
	{
		if (IsValid(InteractionUI) == false)
		{
			//UI 생성 및 기타 값 설정
			CreateUISetting();

			//UI 초기화
			switch (Type)
			{
				case ENPCType::Merchant:
				{
					auto interactionUI = Cast<UUIStore_Game>(InteractionUI);
					interactionUI->Init(ItemList);
					break;
				}
				case ENPCType::Quest:
				{
					auto interactionUI = Cast<UUIQuest_Game>(InteractionUI);
					interactionUI->Init(this);
					break;
				}
				default:
				{
					break;
				}
			}

			//UI 화면 상에 배치 후 설정
			auto widget = Cast<UCanvasPanelSlot>(ClientController->GetHUDWidget()->CanvasPanel_Main->AddChild(InteractionUI));

			if (IsValid(widget))
			{
				if (Type == ENPCType::Quest)
				{
					widget->SetAlignment(FVector2D(0.5f, 0.25f));
					widget->SetSize(FVector2D(400.0f, 600.0f));
					widget->SetPosition(FVector2D(GSystemResolution.ResX / 2, GSystemResolution.ResY / 3));
				}
				else
				{
					widget->SetAutoSize(true);
					widget->SetPosition(FVector2D(GSystemResolution.ResX / 2, GSystemResolution.ResY / 2));
				}
			}

		}
		else //퀘스트인 경우 퀘스트 수락/거절 과정에서 창을 닫아 놓을 수 있음
		{
			switch (Type)
			{
				case ENPCType::Quest:
				{
					auto interactionUI = Cast<UUIQuest_Game>(InteractionUI);
					if (instance->QuestMap->FindQuestSolved(FName(*interactionUI->Data.Name.ToString())) == true)
					{
						return;
					}
					
					interactionUI->Progress();
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
	else {	
		if (IsValid(InteractionUI))
		{
			switch (Type)
			{
				case ENPCType::Merchant:
				{
					RemoveUISetting();
					break;
				}
				case ENPCType::Quest:
				{
					auto interactionUI = Cast<UUIQuest_Game>(InteractionUI);
					auto questType = interactionUI->Data.ProgressType;

					switch (questType)
					{
						case EQuestProgressType::Waiting:
						case EQuestProgressType::Proceeding:
						case EQuestProgressType::Complete:
						{
							interactionUI->Data.ProgressType = EQuestProgressType::End;
							// falls through
						}
						case EQuestProgressType::End:
						{
							RemoveUISetting();
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
}

void ANPC_Game::RemoveUISetting()
{
	InteractionUI->RemoveFromParent();
	InteractionUI = nullptr;

	IsInteraction = false;
	InteractingController->SetInteractedNPC(nullptr);
	InteractingController->SetInputGameModes();
}

void ANPC_Game::CreateUISetting()
{
	InteractionUI = CreateWidget<UUserWidget>(InteractingController, InteractionUIClass);

	IsInteraction = true;
	InteractingController->SetInteractedNPC(this);
	InteractingController->SetInputGameModes(true, true, true);
}

