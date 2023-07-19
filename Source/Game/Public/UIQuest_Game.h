// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "Blueprint/UserWidget.h"
#include "GameInstance_Game.h"
#include "UIQuest_Game.generated.h"

class UTextBlock;
class UButton;
class UListView;

/**
 * 
 */
UCLASS()
class GAME_API UUIQuest_Game : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(class ANPC_Game* NPC);
	void SetVisibilityToggle();

	void SetTextName(FText Name);
	void SetTextStory(FText Story);
	void SetNameRequire(FName Require);
	void RegisterToQuestItem();
	void SetDataList(TArray<TSubclassOf<class AItem_Game> >& Items);

	UFUNCTION(BlueprintCallable)
	bool IsComplete();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FQuestData Data;

	void Progress();

protected:
	UFUNCTION(BlueprintCallable)
	void AcceptQuest();

	UFUNCTION(BlueprintCallable)
	void RefuseQuest();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_QuestName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_QuestStory;

	UPROPERTY(meta = (BindWidget))
	UTextBlock * Text_Require;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Accept;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Refuse;

	UPROPERTY(meta = (BindWidget))
	UListView* ListView_QuestItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TArray<FItemData> DataList;

	UPROPERTY()
	UGameInstance_Game* Instance;
};
