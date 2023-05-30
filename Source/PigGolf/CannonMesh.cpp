// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonMesh.h"
//#include "Math/UnrealMathUtility.h"



UCannonMesh::UCannonMesh():
	Elevation(0)
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCannonMesh::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	Eleva(Elevation, DeltaTime);

}


void UCannonMesh::Eleva(float Gradi,float DeltaTime)
{
	FRotator Vec   = GetComponentRotation();// memorizzo posizione
	float    Pitch = Vec.Pitch;   // estraplo il pitch
	// interpolazione da Pitch a Gradi
	Pitch = FMath::FInterpTo<float>(Pitch, Gradi, DeltaTime, 3.f);
	
	Vec.Pitch = Pitch; // reintegro il nuovo pitch
	SetWorldRotation(Vec);

}