// Fill out your copyright notice in the Description page of Project Settings.


#include "PigZone.h"
#include "Components/SphereComponent.h"

// Sets default values
APigZone::APigZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	SetRootComponent(RootSphere);
	

}

// Called when the game starts or when spawned
void APigZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APigZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

