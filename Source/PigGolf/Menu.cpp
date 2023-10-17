// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PorkInstance.h"

void UMenu::PlayGameClicked()
{
	auto MyInstance = Cast<UPorkInstance>(GetGameInstance());
	MyInstance->NPlayers = Players; // avvio il gioco e i giocatori scelti nel menù vengono scritti sul game instance
	MyInstance->CurrentHole = 0;  

	RemoveFromParent();

	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			// game only, focus on the game !
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData); // now imput mode for this PC is defined 
			PC->SetShowMouseCursor(false);  //mouse cursor disappear
		}
	}

	UGameplayStatics::OpenLevel(this, FName("Level_01"));
}

void UMenu::TutorialClicked()
{
	auto MyInstance = Cast<UPorkInstance>(GetGameInstance());
	MyInstance->NPlayers = Players; // avvio il gioco e i giocatori scelti nel menù vengono scritti sul game instance
	MyInstance->CurrentHole = 0;

	RemoveFromParent();

	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			// game only, focus on the game !
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData); // now imput mode for this PC is defined 
			PC->SetShowMouseCursor(false);  //mouse cursor disappear
		}
	}

	UGameplayStatics::OpenLevel(this, FName("TutorialMap"));
}

void UMenu::MoreClicked()
{
	if (Players < 4 ) Players++;
}

void UMenu::LessClicked()
{
	if (Players > 1) Players--;
}

void UMenu::Setup() // questa funzione la chiamo appena messa a schermo la widget
{
	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			// want use the input only in the menu.. not for the pawn
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget()); // take focus on this widget
			// keep the mouse move outside the window
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputModeData); // now imput mode for this PC is defined 
			PC->SetShowMouseCursor(true);
		}
	}

	// serve a disattivar ei controlli del pawn, visualizzare il mouse, dare focus alla widget , ecc
}

bool UMenu::Initialize() // initialize is just after the constructor 
{
	if (!Super::Initialize()) return false;


	if (PlayGame)  // the name here should be exactlly the same name we have inside the widget
	{
		PlayGame->OnClicked.AddDynamic(this, &ThisClass::PlayGameClicked);
	}

	if (Tutorial)  
	{
		Tutorial->OnClicked.AddDynamic(this, &ThisClass::TutorialClicked);
	}

	if (More)
	{
		More->OnClicked.AddDynamic(this, &ThisClass::MoreClicked);
	}

	if (Less)
	{
		Less->OnClicked.AddDynamic(this, &ThisClass::LessClicked);
	}

	return true;
}
