// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonPawn.h"
#include "CannonMesh.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PigPawn.h"
#include "Components/SphereComponent.h"
#include "PigMode.h"
#include "Kismet/GameplayStatics.h"

static void InitDefaultKeys()
{
	static bool bindingsAdded = false;

	if (bindingsAdded == false)
	{
		bindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::Gamepad_LeftY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::Gamepad_LeftX, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Up, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Down, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Gamepad_RightY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Left, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Right, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Gamepad_RightX, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::EKeys::MouseY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("FireButton", EKeys::SpaceBar));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("FireButton", EKeys::Gamepad_FaceButton_Bottom));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("CameraButton", EKeys::M));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("CameraButton", EKeys::Gamepad_FaceButton_Right));
	}
}

void ACannonPawn::ViewMap()
{
	auto  MC = Cast<APlayerController>(GetController());

	if (GetWorldTimerManager().IsTimerActive(WaitCam)) return;

	GetWorldTimerManager().SetTimer(WaitCam, this, &ThisClass::Nulla, InterpTime, false);

	if (!bCamera)
	{
		if (MC) MC->SetViewTargetWithBlend(CameraMap, InterpTime, EViewTargetBlendFunction::VTBlend_Cubic);
		bCamera = true;
	}
	else
	{
		if (MC) MC->SetViewTargetWithBlend(this, InterpTime, EViewTargetBlendFunction::VTBlend_Cubic);
		bCamera = false;
	}
}

void ACannonPawn::AddGold()
{
	if (Gold < 30) Gold++; // ogni 30 monete si recuperaun tiro
	else
	{
		Gold = 0;
		PGM->SetShots(-1);
	}
}

// Sets default values
ACannonPawn::ACannonPawn():
	Power(0.f),
	MaxPower(100.f),
	bFirePressed(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);	
}

float ACannonPawn::GetPitchRotation()
{
	return int32(Cannon->GetComponentRotation().Pitch);
}

// Called when the game starts or when spawned
void ACannonPawn::BeginPlay()
{
	Super::BeginPlay();

	if (TrackType)  // tipo per le palline utili all'aiming
	{
		for (int i = 0; i < balls; i++)
		{   // creo gli elementi e li inserisco nell'array
			auto nuovo = GetWorld()->SpawnActor<AActor>(TrackType);

			nuovo->SetActorHiddenInGame(true);   // nascondo l'oggetto palla
			nuovo->SetActorEnableCollision(false);
			auto Sfera = nuovo->FindComponentByClass<UPrimitiveComponent>();
    		if (Sfera) Sfera->SetSimulatePhysics(false);

    		Trackers.Add(nuovo);
		}
	}
}

// Called every frame
void ACannonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFirePressed)
	{
		if (Power < MaxPower) Power += 25 * DeltaTime;

		if (PorkType == EPorkType::EPT_Aiming)
		{
			if (Cadenza > 0) Cadenza -= DeltaTime;
			else // quando la Cadenza arriva a zero creo un traker bullet
			{
				if (Trackers.Num() > 0) // se ho ancora elementi
				{
					auto Tr = Trackers.Pop(); // estraggo il puntatore al primo elemento e l'array decresce di 1
					TrackersOut.Add(Tr); // inserisco l'oggetto prezo da Trakers nel secondo array

					// ri attivo collisioni e fisica sul tracker
					Tr->SetActorHiddenInGame(false);
					Tr->SetActorEnableCollision(true); // riabilito visibilità e collisioni dell'oggetto nascosto
					auto Sfera = Tr->FindComponentByClass<UPrimitiveComponent>();

					if (Sfera)
					{
						Sfera->SetSimulatePhysics(true); // riattivo la fisica 
						auto SpawnLocation = Cannon->GetComponentLocation() + Cannon->GetComponentRotation().Vector() * 300;
						// lancio il proiettile
						Tr->SetActorLocation(SpawnLocation);
						auto FirePower = Cannon->GetComponentRotation().Vector() * (Power * Moltiplicatore)  * Sfera->GetMass() * 50;
						Sfera->AddImpulse(FirePower); // vado direttamente sul componente che avevvo trovato
					}
		
					Cadenza = CadenzaMax;
				}
			}
		}
	}
}

// Called to bind functionality to input
void ACannonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	InitDefaultKeys();

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &ThisClass::LockUpAtRate);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ThisClass::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	
	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &ThisClass::FirePress);
	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Released, this, &ThisClass::FireRelease);

	PlayerInputComponent->BindAction("CameraButton", EInputEvent::IE_Pressed, this, &ThisClass::ViewMap);
}

void ACannonPawn::FirePress()
{
	if (Maiale) return;
	bFirePressed = true;
}

void ACannonPawn::FireRelease()
{
	bFirePressed = false;
	if (Maiale) return; // se è già stato creato un proiettile 

	auto SpawnLocation = Cannon->GetComponentLocation() + Cannon->GetComponentRotation().Vector() * 300;

    // Maiale  = GetWorld()->SpawnActor<APigPawn>(MyPig, SpawnLocation, Cannon->GetComponentRotation());
	// al posto dello spawn devo cercare il game mode e la variabile PawnProiettile

	Maiale = PGM->PawnProiettile;  // cerca il proiettile sul game mode

	PGM->SetBulletOnHUD(Maiale); // assegno il puntatore all'HUD
	PGM->SetShots(1);

	Maiale->SetCannone(this); // informo il proiettile di chi lo ha sparato 

    Maiale->Inizializza();

	Maiale->SetActorLocation(SpawnLocation);
	Maiale->SetActorRotation(Cannon->GetComponentRotation());

	FPredictProjectilePathParams MyParam;
	FPredictProjectilePathResult MyResult;

	//MyResult.
	//MyParam.

	//UGameplayStatics::PredictProjectilePath();

	if (!Maiale->Sfera)
	{
		UE_LOG(LogTemp, Error, TEXT("missing sphere"));
		return;
	}
	//  devo disattivare tutte le sfere e metterle nell'array 

	for (int i = 0; i < TrackersOut.Num(); i++) //tutte le sfere che erano uscite tornano nell'array Trackers
	{
		auto Tr = TrackersOut.Pop(); // estraggo il puntatore al primo elemento e l'array decresce di 1

		Tr->SetActorHiddenInGame(true);   // nascondo l'oggetto palla
		Tr->SetActorEnableCollision(false);
		auto Sfera = Tr->FindComponentByClass<UPrimitiveComponent>();
		if (Sfera) Sfera->SetSimulatePhysics(false);

		Trackers.Add(Tr);
	}

	//

	auto FirePower = Cannon->GetComponentRotation().Vector() * Power * Maiale->Sfera->GetMass() * 50;
	Power = 0;

	Maiale->Sfera->AddImpulse(FirePower);

	// chiedo al controller di posseder il proiettile appena creato

	auto  MC = Cast<APlayerController>(GetController()); // trovo il controller

	if (MC) MC->SetViewTargetWithBlend(Maiale, InterpTime, EViewTargetBlendFunction::VTBlend_Linear);
	
	FTimerHandle time;
	GetWorldTimerManager().SetTimer(time, this, &ThisClass::ChangePawn, InterpTime, false);

}

void ACannonPawn::ChangePawn()
{
	if (!Maiale) return;
	auto M = GetController();
	M->Possess(Maiale); // gli dico di possedere il maiale 
}


void ACannonPawn::MoveForward(float value) // guarda alto basso
{
	if (value == 0) return;

	Cannon->Elevation += value*45.f*GetWorld()->GetDeltaSeconds();
	Cannon->Elevation = FMath::Clamp<float>(Cannon->Elevation, -20, 70);
}

void ACannonPawn::MoveRight(float value)  // guarda sinistra destra
{
	if (value == 0) return;
	FRotator Rot = GetActorRotation();
	SetActorRotation(Rot + FRotator(0, value*45* GetWorld()->GetDeltaSeconds(), 0));
}

void ACannonPawn::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ACannonPawn::Turn(float value)
{
	AddControllerYawInput(value);
}

