// Fill out your copyright notice in the Description page of Project Settings.


#include "PigPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "CannonPawn.h"
#include "PigMode.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PigZone.h"


// Sets default values


static void InitDefaultKeys()
{
	static bool bindingsAdded = false;

	if (bindingsAdded == false)
	{
		bindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Up, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Down, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Gamepad_RightY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Left, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Right, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Gamepad_RightX, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::EKeys::MouseY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("FireButton", EKeys::SpaceBar));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("FireButton", EKeys::Gamepad_RightTrigger));

	}
}

APigPawn::APigPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sfera = CreateDefaultSubobject<USphereComponent>(TEXT("Sfera"));
	SetRootComponent(Sfera);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Sfera);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APigPawn::BeginPlay()
{
	Super::BeginPlay();
	Sfera->OnComponentHit.AddDynamic(this, &ThisClass::MyHit);	
}

void APigPawn::Inizializza()
{
	Peti = MaxPeti;
	UGameplayStatics::PlaySoundAtLocation(this, GrugnitoBase, GetActorLocation());

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true); // riabilito visibilità e collisioni dell'oggetto nascosto
	Sfera->SetSimulatePhysics(true); // riattivo la fisica 
}


void APigPawn::Disattiva()
{
	// se al momento di disattivare il proiettile è fuori dalla sfera, torna al punto originale 

	TArray<AActor*> OverlappingActors;
	MeshRef->GetOverlappingActors(OverlappingActors);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false); 
	Sfera->SetSimulatePhysics(false); 
	bHisLanded = false; // nasconde e resetta proiettile

	
	bool AddPenalty = true;

	for (int i = 0; i < OverlappingActors.Num(); i++)
	{
		auto MyZone = Cast<APigZone>(OverlappingActors[i]);


		if (MyZone) // ho trovato attorno a me la zona di atterraggio
		{  

			Cannone->GetBody()->SetSimulatePhysics(true); // attiva la fisica per il cannone
			
			//se ho trovato una zona posso spostare il cannone
			//devo verificare la presenza di oggetti nelle vicinanze 
			FVector NewLoc{ GetActorLocation() + FVector(0, 0, 310.f) };
			  
			if (!VerifySphere(NewLoc))
			{
				Cannone->SetActorLocation(NewLoc);
			}
			else
			{
				float dist = 200;
				// cerco una zona lontana da muri eventuali
				if (!VerifySphere(NewLoc + FVector(dist, 0, 0)))
				      {
					  NewLoc += FVector(dist, 0, 0);
					  Cannone->SetActorLocation(NewLoc);
			     	  }
				else if (!VerifySphere(NewLoc + FVector(-dist, 0, 0)))
					  {
					  NewLoc += FVector(-dist, 0, 0);
					  Cannone->SetActorLocation(NewLoc);
					  }
				else if (!VerifySphere(NewLoc + FVector(0, dist, 0)))
				      {
					  NewLoc += FVector(0, dist, 0);
					  Cannone->SetActorLocation(NewLoc);
				      }
				else if (!VerifySphere(NewLoc + FVector(0, -dist, 0)))
				      {
					  NewLoc += FVector(0, -dist, 0);
					  Cannone->SetActorLocation(NewLoc);
				      }
			}

			if (BigFart && Peto)
			{

				UGameplayStatics::PlaySoundAtLocation(this, Peto, GetActorLocation());

				auto PC = Cast<APlayerController>(GetController());
				if (PC) // cerco il putno di spawn solo se il controller c'è 
				{
					FVector  Loc;
					FRotator Rot;

					PC->GetPlayerViewPoint(Loc, Rot);

					auto NewPos = Loc + Rot.Vector() * 100.f; //posiziono un metro di fronte alla camera

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						this,
						BigFart,
						NewPos,
						GetActorRotation(),
						FVector(0.7f),
						true,
						true);
				}
			}

			AddPenalty = false; // nessuna penalità ho trovato la zona giusta
			// chiedo al Game Mode di cambiare giocatore
			PGM->ChangePlayer(Cannone); // il proiettile passa Cannon (il cannone attuale al GAME MODE)


			break;
		}
	
	}

	if (AddPenalty)  
	    {  // penalità riabilito il fuoco
		Cannone->Maiale = nullptr;
		PGM->SetShots(1);
		BlendCamera(Cannone);  // blend camera al cannone originale solo se il tiro 
		                       // è mancanto
	    } 
	// un punto in più per aver sbagliato zona  

	// attivo la fisica al cannone e lo sposto poco sopra la posizione del proiettile
	
	//Cannone->Maiale = nullptr; // il valore del proiettile torna nullo in modo da poter sparare di nuovo
	PGM->SetBulletOnHUD(nullptr); // tolgo il riferimento dalla widget
}

void APigPawn::Water()
{
	if (Splash) // play sound
	{
		UGameplayStatics::PlaySoundAtLocation(this, Splash, GetActorLocation());
	}

	if (WaterVFX) // play sound
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			WaterVFX,
			GetActorLocation()+FVector(0,0,50),
			GetActorRotation(),
			FVector(0.7f),
			true,
			true);
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	Sfera->SetSimulatePhysics(false);
	bHisLanded = false;
	Cannone->GetBody()->SetSimulatePhysics(true);
	PGM->SetShots(1); // un punto in più per aver sbagliato zona
	// attivo la fisica al cannone e lo sposto poco sopra la posizione del proiettile
	Cannone->Maiale = nullptr; // il valore del proiettile torna nullo in modo da poter sparare di nuovo
	PGM->SetBulletOnHUD(nullptr); // tolgo il riferimento dalla widget

	BlendCamera(Cannone);

}

bool APigPawn::VerifySphere(FVector NewLoc)
{
	FHitResult Hit;
	FQuat Rot{ FRotator(0).Quaternion() };

	bool Found = GetWorld()->SweepSingleByChannel(Hit, NewLoc,
		NewLoc + FVector(0, 0, 1),
		Rot,
		ECC_Visibility,
		FCollisionShape::MakeSphere(180.f));
	
	return Found;
}

void APigPawn::BlendCamera(AActor* BlendTo, float BlendTime, bool Destroy)
{	
		//auto  MC = Cast<APlayerController>(GetController()); 
	    auto MC = GetWorld()->GetFirstPlayerController();

		FTimerHandle MyTime;

		GetWorldTimerManager().SetTimer(MyTime, this, &ThisClass::ReturnCannon, BlendTime, false);

		if (MC)
		{
			MC->SetViewTargetWithBlend(BlendTo, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}
}

void APigPawn::ReturnCannon() // chiamata alla fine
{
	//auto M = GetController();
	auto M = GetWorld()->GetFirstPlayerController();
	if (M)
	{
		M->Possess(Cannone); // il controller torna a posseder il Cannone
		SetActorLocation(FVector(0, 0, -9999));
	}
}

// Called every frame
void APigPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHisLanded) return;

	FVector  dir = GetVelocity(); // prendo la velocita compresniva di direzione;
	if (dir.Size() <= 0) return;

	FRotator rot   = dir.Rotation(); // conversione di vettore in rotazione
	FRotator MyRot = GetActorRotation();
	MyRot.Pitch = rot.Pitch; // assegno solo il ptich (rotazione su asse Y)
	SetActorRotation(MyRot);

}

// Called to bind functionality to input
void APigPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InitDefaultKeys();

	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);

	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &ThisClass::FirePress);

}

void APigPawn::FirePress()
{
	if (bHisLanded || Peti <= 0) return;

	Peti--;

	auto Vel = GetVelocity(); 
	// trovo la velocity per aggiungere una spinta in X
	Vel.Normalize();
	auto FirePower = FVector(Vel.X, Vel.Y, 1) * Sfera->GetMass() * 50 * Pow;
	Sfera->AddImpulse(FirePower);
	AttachFart();
}

void APigPawn::AttachFart()
{
	if (!Fart ||!Peto) return;

	UGameplayStatics::PlaySoundAtLocation(this, Peto, GetActorLocation());

	UNiagaraFunctionLibrary::SpawnSystemAttached(Fart,
		Sfera,
		NAME_None,
		GetActorLocation(),
		GetActorRotation(),
		EAttachLocation::KeepWorldPosition, true, true);
}



void APigPawn::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void APigPawn::Turn(float value)
{
	AddControllerYawInput(value);
}

void APigPawn::MyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (!bHisLanded && !bBouncing) // se è stato attivato un bounce non collido a terra.
	{
		bHisLanded = true;
		UGameplayStatics::PlaySoundAtLocation(this, GrugnitoSoffre, GetActorLocation());

		FTimerHandle time; // il timer handle è temporaneo perchè non devo fare modifiche successive
		GetWorldTimerManager().SetTimer(time, this, &ThisClass::Disattiva, 2.f, false);

	}


}