// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable.generated.h"

UENUM(BlueprintType)
enum class ECollectableType : uint8
{
	ECT_Gold   UMETA(DisplayName = "Gold"),
	ECT_Fart   UMETA(DisplayName = "Fart"),
	ECT_Pow    UMETA(DisplayName = "Pow"),
	ECT_Aim    UMETA(DisplayName = "Aim"),
	ECT_MAX    UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class PIGGOLF_API ACollectable : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "Collectable")
		ECollectableType CollectableType { ECollectableType::ECT_Gold };

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* Collision {nullptr };

	UPROPERTY(VisibleAnyWhere)
	class USphereComponent* MySphere;

	UPROPERTY(EditAnyWhere, Category = "Sound")
	class USoundCue* Coin;

	UPROPERTY(EditAnyWhere, Category = "Vfx")
	class UNiagaraSystem* CoinVFX;


public:	
	// Sets default values for this actor's properties
	ACollectable();

	virtual void NotifyActorBeginOverlap(AActor* Other) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
