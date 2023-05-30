// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CannonMesh.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class PIGGOLF_API UCannonMesh : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	

	UCannonMesh();

	void Eleva(float Gradi, float DeltaTime);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float Elevation;

};


