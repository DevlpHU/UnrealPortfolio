// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLoadingScreen.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"
#include "Components/CircularThrobber.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!
struct FGameLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FGameLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

class SGameLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		// Load version of the logo with text baked in, path is hardcoded because this loads very early in startup
		static const FName LoadingScreenName(TEXT("/Game/Maps/InfinityBladeGrassLands/Effects/FX_Textures/Tile/T_Black_32.T_Black_32"));

		LoadingScreenBrush = MakeShareable(new FGameLoadingScreenBrush(LoadingScreenName, FVector2D(2000.0f, 1150.0f)));
		
		FSlateFontInfo FontInfo;	
		FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/Font/Roboto.Roboto"));
		FontInfo.Size = 50.0f;

		ChildSlot
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
		        .HAlign(HAlign_Center)
		        .VAlign(VAlign_Center)
		        [
		        	SNew(SImage)
		        	.Image(LoadingScreenBrush.Get())
		        ]
		        + SOverlay::Slot()
		        .HAlign(HAlign_Center)
		        .VAlign(VAlign_Center)
		        [
		        	SNew(STextBlock)
		        	.Text(FText::FromString(TEXT("Loading")))
					.Font(FontInfo)
		        ]
		        + SOverlay::Slot()
		    	.HAlign(HAlign_Center)
		    	.VAlign(VAlign_Center)
		    	[
		    		SNew(SCircularThrobber)
		    		.Period(1.0f)
		    		.Visibility(EVisibility::SelfHitTestInvisible)
		    	]
			];
	}

private:
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FGameLoadingScreenModule : public IGameLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		LoadObject<UObject>(nullptr, TEXT("/Game/Maps/InfinityBladeGrassLands/Effects/FX_Textures/Tile/T_Black_32.T_Black_32") );
		LoadObject<UObject>(nullptr, TEXT("/Game/Font/DroidSansMono.DroidSansMono"));

		CreateScreen();
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		if (IsMoviePlayerEnabled())
		{
			FLoadingScreenAttributes LoadingScreen;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
			LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
			LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
			LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
			LoadingScreen.WidgetLoadingScreen = SNew(SGameLoadingScreen);
			GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		}
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}

	virtual void CreateScreen()
	{
		if (IsMoviePlayerEnabled())
		{
			FLoadingScreenAttributes LoadingScreen;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
			LoadingScreen.MinimumLoadingScreenDisplayTime = 3.f;
			LoadingScreen.WidgetLoadingScreen = SNew(SGameLoadingScreen);
			GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		}
	}
};

IMPLEMENT_GAME_MODULE(FGameLoadingScreenModule, GameLoadingScreen);
