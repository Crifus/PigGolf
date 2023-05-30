// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBoard.generated.h"

class UButton;


UCLASS()
class PIGGOLF_API UScoreBoard : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(meta = (BindWidget))
		UButton* PlayGame;

	UFUNCTION()
		void NextLevelClicked();

	int32 LastScore[4]{ 0,0,0,0 }; // score for 4 players
	int32 TotalScore[4]{ 0,0,0,0 };

public:

	UFUNCTION(BlueprintCallable)
		void Setup();


	UPROPERTY(BlueprintReadOnly)
		int32 Players { 1 };

	UFUNCTION(BlueprintPure)
	int32 GetLastScore1() { return LastScore[0]; };
	UFUNCTION(BlueprintPure)
	int32 GetTotalScore1() { return TotalScore[0]; };
	UFUNCTION(BlueprintPure)
	int32 GetLastScore2() { return LastScore[1]; };
	UFUNCTION(BlueprintPure)
	int32 GetTotalScore2() { return TotalScore[1]; };
	UFUNCTION(BlueprintPure)
	int32 GetLastScore3() { return LastScore[2]; };
	UFUNCTION(BlueprintPure)
	int32 GetTotalScore3() { return TotalScore[2]; };
	UFUNCTION(BlueprintPure)
	int32 GetLastScore4() { return LastScore[3]; };
	UFUNCTION(BlueprintPure)
	int32 GetTotalScore4() { return TotalScore[3]; };

	virtual bool Initialize() override;
};
