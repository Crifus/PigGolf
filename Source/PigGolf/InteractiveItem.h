// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Circle UMETA(DisplayName = "Circle"),
	EIT_Bounce UMETA(DisplayName = "Bounce"),
	EIT_Final  UMETA(DisplayName = "Final"),
	EIT_Water  UMETA(DisplayName = "Water"),
	EIT_MAX    UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PIGGOLF_API AInteractiveItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* Collision{ nullptr };

	UPROPERTY(EditAnyWhere) // kind of interaction 
	float Power{ 1000.f };

public:	
	// Sets default values for this actor's properties
	AInteractiveItem();

	virtual void NotifyActorBeginOverlap(AActor* Other) override;

	UPROPERTY(EditAnyWhere) // kind of interaction 
		EItemType Kind {
		EItemType::EIT_Circle
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
