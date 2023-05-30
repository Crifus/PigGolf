// Fill out your copyright notice in the Description page of Project Settings.


#include "PigMode.h"
#include "PigPawn.h"
#include "CannonPawn.h"
#include "InteractiveItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PorkInstance.h"

void APigMode::ChangePlayer(ACannonPawn* Player)
{
	// il giocatore informa il GM di aver finito, il
	// GM cointrolla chi è il più lontano dalla bandiera
	// e gli passa il controllo 

	if (FirstLoop) // non ho ancora abilitato tutti quanti
	{
		for (int i = 0; i < NPlayers; i++)
		{
			if (!Players[i]->enabled) // cerco il primo giocatore non abilitato
			{ // lo abilito e piazzo nella locazione di player start

				SetPlayer(i,true);

				return; // ho trovato un giocatore inattivo , esco dalla funzione
			}

		}

    	FirstLoop = false;
		ChangePlayer(Player); // chiamo in maniera ricorsiva la funzione
	}
	else
	{
		float DiffMax = 0;
		int32 SelectedPlayer = 0;


	    for(int i = 0; i < NPlayers;i++)
		{ 

			if (!Players[i]->hasgoal)
			{
				auto diff = FMath::Abs((Players[i]->GetActorLocation() - EndPoint).Size());

				if (diff > DiffMax)
				{
					DiffMax = diff; // controllo che il nuovo pawn abbia una lontananza maggiore di quella max attuale
					SelectedPlayer = i;
				}
			}
		}	

		if (DiffMax == 0) // se diffMax resta 0 significa che tutti i cannoni hanno il goal assegnato
		{
			auto MyInstance = Cast<UPorkInstance>(GetGameInstance());

			for (int i = 0; i < NPlayers; i++)
			{
				MyInstance->TotalScore[i] += Shots[i];   //per ogni giocatore aggiorno
				MyInstance->LastScore[i]   = Shots[i];   //il numero dei tiri nel game instace.
				MyInstance->Gold[i] = Players[i]->GetGold(); // l'oro è memorizzato durante il gioco sul pawn e non sul game mode come lo score
				// memorizzo sul GI sia i tiri totali della partita che quelli relativi all'ltima buca
			
			}

			UGameplayStatics::OpenLevel(this,FName("Board")); // apro il livello che mostra i risultati

		}

		SetPlayer(SelectedPlayer,false);
		// cerco il cannone più lontano dal goal e gli assegno il controllo

	}

}


void APigMode::SetPlayer(int i,bool reset) // chiamata alla fine
{

	if (reset) {

		Players[i]->SetActorHiddenInGame(false);
		Players[i]->SetActorEnableCollision(true);
		Players[i]->enabled = true;
		Players[i]->GetBody()->SetSimulatePhysics(true);
		Players[i]->SetActorLocation(StartPoint); 
		Players[i]->SetActorRotation(StartRotation);
	}
	SetCannonOnHUD(Players[i]);  //assegno all' HUD il nuovo giocatore 

	auto  MC = GetWorld()->GetFirstPlayerController();

	FTimerHandle MyTime;
	GetWorldTimerManager().SetTimer(MyTime, this, &ThisClass::PossessCannon, 2.f, false);

	if (MC)
	{

		MC->SetViewTargetWithBlend(Players[i], 2.f, EViewTargetBlendFunction::VTBlend_Cubic);
		PrevPlayer = CurrentPlayer;
		CurrentPlayer = i;
		UE_LOG(LogTemp, Warning, TEXT("Par  %i"), Par);
		SetShotsOnHUD(Shots[CurrentPlayer], Par); 

	}
}



void APigMode::PossessCannon() // chiamata alla fine
{
	auto M = GetWorld()->GetFirstPlayerController();
	if (M)
	{
		Players[PrevPlayer]->Maiale = nullptr; // quando cambio pawn il vecchio giocatore riabilita il fuoco
		M->Possess(Players[CurrentPlayer]);    // il controller torna a posseder il Cannone	
	}
}





void APigMode::SetShots(int32 Add, int32 NewPar)
{
	Shots[CurrentPlayer] += Add;
	if (NewPar > 0) Par = NewPar;
	SetShotsOnHUD(Shots[CurrentPlayer], Par);
}

APigMode::APigMode()
{

}

void APigMode::BeginPlay() 
{
	Super::BeginPlay();


	auto MyInstance = Cast<UPorkInstance>(GetGameInstance());
	if (!MyInstance) return;

	UE_LOG(LogTemp, Error, TEXT("Current %i "), MyInstance->CurrentHole);

	Par = MyInstance->Par[MyInstance->CurrentHole];  // dal game instance leggo quale è il par per la buca corrente 
	NPlayers = MyInstance->NPlayers; // dal game instance leggiamo il numero di giocatori
	UE_LOG(LogTemp, Error, TEXT("Par %i "), Par);
	Shots.Add(0); // score del giocatore 1 (sempre presente)

	SetShotsOnHUD(Shots[CurrentPlayer], Par);

	// creazione del proiettile (condiviso)

	if (Maiale) // se ho selezionato un proiettile
	{
		PawnProiettile = GetWorld()->SpawnActor<APigPawn>(Maiale);
		PawnProiettile->PGM = this; // il game mode passa il suo puntatore al proiettile
		PawnProiettile->SetActorHiddenInGame(true);
		PawnProiettile->SetActorEnableCollision(false);
		PawnProiettile->Sfera->SetSimulatePhysics(false); 
	}
	// memorizzo locazione player Start

	
	//salviamo lo start point e il punto finale
	StartPoint = FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorLocation();
	StartRotation = FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorRotation();

	TArray<AActor*> ItemsFound;
	UGameplayStatics::GetAllActorsOfClass(this, AInteractiveItem::StaticClass(), ItemsFound);

	for (int i = 0; i < ItemsFound.Num(); i++)
	{
	auto tmp =	Cast<AInteractiveItem>(ItemsFound[i]);

		if (tmp) if (tmp->Kind == EItemType::EIT_Final)
		{
			EndPoint = tmp->GetActorLocation();
 	    }

	}

	//EndPoint

	// memorizzo nell'array il primo giocatore

	auto Pawn = GetWorld()->GetFirstPlayerController()->GetPawn(); // 
	auto FirstPlayer = Cast<ACannonPawn>(Pawn);
	FirstPlayer->PGM = this; 
	FirstPlayer->enabled = true;
	FirstPlayer->SetGold(MyInstance->Gold[0]);
	Players.Add(FirstPlayer);
	
	// controllo il numero di giocatori
	// se è superiore ad 1 creo gli altri e li inserisco nell'array

	for (int i = 1; i < NPlayers; i++) //creo un cannone per ogni giocatore extra
	{
		if (BigMaiale) // se ho selezionato una classe di tipo CannonPawn
		{
			auto temp = GetWorld()->SpawnActor<ACannonPawn>(BigMaiale,FVector(0,0,9999),FRotator(0));
			if (temp)
			{
				Players.Add(temp);
				Shots.Add(0);
				temp->PGM = this;
				temp->GetBody()->SetMaterial(3, Materials[i - 1]);
				temp->SetActorHiddenInGame(true);
				temp->SetActorEnableCollision(false);
				temp->GetBody()->SetSimulatePhysics(false);
				temp->SetGold(MyInstance->Gold[i]);
			}
		}
	}

}
