// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PigZone.generated.h"

UCLASS()
class PIGGOLF_API APigZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APigZone();

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	class USphereComponent* RootSphere{ nullptr };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
