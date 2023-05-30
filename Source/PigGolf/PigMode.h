// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PigMode.generated.h"
/**
 * 
 */
UCLASS()
class PIGGOLF_API APigMode : public AGameMode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APigPawn>    Maiale; // tipo di proiettile

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACannonPawn> BigMaiale; // tipo di cannone

	UPROPERTY(VisibleAnywhere) // visibile solo per debug
	TArray<ACannonPawn*>  Players; // array di puntatori per i vari giocatori

	UPROPERTY(EditAnywhere)
	int32 NPlayers{ 1 };

	TArray<int32> Shots; // i tiri effettuati da ogni giocatore  per la mappa specifica

	int32 CurrentPlayer{ 0 };

	int32 PrevPlayer;  // memorizzo il codice del giocatore precedente (al momento di cambiare giocatore)

	bool FirstLoop{ true }; // al primo giro tutti i giocatori fanno un tiro dopodiche tal variabile diventa falsa

	FVector StartPoint; // locazione dove memorizzare la zona del player start

	FVector EndPoint;  // locazione della bandierina per questa mappa

	FRotator StartRotation; // starting rotation (per piazzare nel verso corretto i pawn dei giocatori dopo il primo)

public:

	void PossessCannon();

	void ChangePlayer(ACannonPawn* Player); 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Par { 4 }; // il Par di default, verra poi cambiato a seconda dei valori sul Game Instance

	UFUNCTION(BlueprintCallable)
    void SetShots(int32 Add,int32 NewPar = 0);

	UFUNCTION(BlueprintImplementableEvent) // funzione per settare sull' HUD Par e Tiri effettuati
	void SetShotsOnHUD(int32 NewShots,int32 NewPar);

	UFUNCTION(BlueprintImplementableEvent) // funzione per settare sull' HUD Par e Tiri effettuati
	void SetCannonOnHUD(ACannonPawn* MC);

	UPROPERTY(EditAnyWhere, Category = "Materiali")
	TArray<UMaterialInterface*> Materials;

	void SetPlayer(int i, bool reset);

	APigPawn* PawnProiettile;

	APigMode();

	virtual void BeginPlay() override; 
	// funzione implementata in BP che assegna come proiettile quello passato da argomento
	UFUNCTION(BlueprintImplementableEvent)
	void SetBulletOnHUD(APigPawn* MB);

};
