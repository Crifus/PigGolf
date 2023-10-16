// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PorkInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIGGOLF_API UPorkInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPorkInstance();

	UPROPERTY(BlueprintReadWrite)
	int32 NPlayers{ 1 };

	
	int32 TotalScore[4]{ 0,0,0,0 }; // lo score complessivo per i giocatori
	// lo utilizzeremo sia per il valore complessivo che per i punteggi della singola buca
	int32 Gold[4]{ 0,0,0,0 };

	int32 LastScore[4]{ 0,0,0,0 }; // lo score complessivo per i giocatori

	int32 CurrentHole{ 0 };

	int32 Par[3]{ 3,5,4 }; // incrementare questo array per ogni nuova buca creata.

};
