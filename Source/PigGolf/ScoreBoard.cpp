// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreBoard.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PorkInstance.h"

void UScoreBoard::NextLevelClicked()
{
	auto MyInstance = Cast<UPorkInstance>(GetGameInstance());
	
	MyInstance->CurrentHole++;

	auto current{ MyInstance->CurrentHole};
	
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

	switch (current)
	{
	case 0:
		UGameplayStatics::OpenLevel(this, FName("Level_01"));
	break;

	case 1:
		UGameplayStatics::OpenLevel(this, FName("EndGame"));
	break;

	case 2:
		UGameplayStatics::OpenLevel(this, FName("Level_03"));
	break;

	case 3:
		UGameplayStatics::OpenLevel(this, FName("EndGame"));
	break;

	}
}

void UScoreBoard::Setup()
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
}

bool UScoreBoard::Initialize()
{
	if (!Super::Initialize()) return false;

	auto MyInstance = Cast<UPorkInstance>(GetGameInstance());

	if (MyInstance)
	{
		for (int i = 0; i < 4; i++)
		{
			LastScore[i]  = MyInstance->LastScore[i];
			TotalScore[i] = MyInstance->TotalScore[i];
		}
	}


	if (PlayGame)  // the name here should be exactlly the same name we have inside the widget
	{
		PlayGame->OnClicked.AddDynamic(this, &ThisClass::NextLevelClicked);
	}

	return true;
}
