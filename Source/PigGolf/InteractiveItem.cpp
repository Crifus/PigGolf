// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItem.h"
#include "PigPawn.h"
#include "Components/SphereComponent.h"

// Sets default values
AInteractiveItem::AInteractiveItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

}

void AInteractiveItem::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	auto PP{ Cast<APigPawn>(Other) };

	if (PP)
	{
		switch (Kind)
		{
			case EItemType::EIT_Circle:
			{

			//PP->GetVelocity();
				auto Vel = PP->GetVelocity();
				// trovo la velocity per aggiungere una spinta in X
				Vel.Normalize();

				//auto FirePower = FVector(Vel.X, Vel.Y, 1) * PP->Sfera->GetMass() * Power;

				PP->Sfera->SetPhysicsLinearVelocity(FVector(0));
				auto FirePower = (GetActorForwardVector() * 10 + FVector(1, 0, 0.2)) * PP->Sfera->GetMass() * Power;

				PP->SetActorRotation(FirePower.Rotation());

				PP->Sfera->AddImpulse(FirePower);
				PP->AttachFart();
			}
			break;

			case EItemType::EIT_Bounce:
			{
				if (PP->GetLanded()) return;

				auto Vel = PP->GetVelocity();
				// trovo la velocity per aggiungere una spinta in X
				Vel.Normalize();

				PP->Sfera->SetPhysicsLinearVelocity(FVector(0));
				auto FirePower = (GetActorForwardVector()+FVector(0, 0, 1)) * PP->Sfera->GetMass() * Power;

				PP->SetActorRotation(FirePower.Rotation());

				//UE_LOG(LogTemp, Error, TEXT(" forward %s"), *GetActorForwardVector().ToString());

				PP->Sfera->AddImpulse(FirePower);
				PP->AttachFart();

				PP->bBouncing = true;

				FTimerHandle TM;

				GetWorldTimerManager().SetTimer(TM,PP, &APigPawn::ResetBounce, 0.2f,false);

			}
			break;

			case EItemType::EIT_Water:
			{
				PP->Water();
			}
			break;


			case EItemType::EIT_Final:
			{

			}
			break;	
		}
	}
}

// Called when the game starts or when spawned
void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

