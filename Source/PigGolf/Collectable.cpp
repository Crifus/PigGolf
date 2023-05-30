// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"
#include "PigPawn.h"
#include "CannonPawn.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	MySphere  = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	MySphere->SetupAttachment(Collision);
	MySphere->SetSphereRadius(30.f);

}

void ACollectable::NotifyActorBeginOverlap(AActor* Other)
{
	auto Pork = Cast<APigPawn>(Other);

	if (!Pork) return;

	switch (CollectableType)
	{

	case ECollectableType::ECT_Gold:

		Pork->GetCannone()->AddGold();

		UGameplayStatics::PlaySoundAtLocation(this, Coin, GetActorLocation());

	break;

	case ECollectableType::ECT_Fart:

		Pork->AddFart();

		UGameplayStatics::PlaySoundAtLocation(this, Coin, GetActorLocation());
	break;

	case ECollectableType::ECT_Pow:
		
		Pork->GetCannone()->AddPower();
		UGameplayStatics::PlaySoundAtLocation(this, Coin, GetActorLocation());

	break;

	case ECollectableType::ECT_Aim:

		//UE_LOG(LogTemp, Error, TEXT("sono qui!!!!"));
		Pork->GetCannone()->SetPorkType(EPorkType::EPT_Aiming);
		UGameplayStatics::PlaySoundAtLocation(this, Coin, GetActorLocation());

	break;

	case ECollectableType::ECT_MAX:

	break;



	}

	Destroy();


}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetActorRotation() + FRotator(0, 180 * DeltaTime, 0));


}

