// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CannonPawn.generated.h"

UENUM(BlueprintType)
enum class EPorkType : uint8
{
	EPT_Normal     UMETA(DisplayName = "Normal"), 
	EPT_Disabled   UMETA(DisplayName = "Disabled"),
	EPT_Aiming     UMETA(DisplayName = "Aiming"),
	ECT_MAX        UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class PIGGOLF_API ACannonPawn : public APawn
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Body{ nullptr };

	// per colpa di un bug questo oggetto lo assegno da BP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UCannonMesh* Cannon { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm{ nullptr };

	float Power;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Track", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Trackers;//array contenente i riferimenti a tutti gli oggetti traker

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Track", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> TrackersOut; //conterrà le sfere lanciate 

	UPROPERTY(EditAnyWhere)
	TSubclassOf<AActor> TrackType;

	UPROPERTY(EditAnywhere)
	float MaxPower;

	bool bFirePressed;

	UPROPERTY(EditAnywhere)
	float InterpTime{ 1.f };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* CameraMap;

	void ViewMap();

	bool bCamera{ false };

	FTimerHandle WaitCam;

	void Nulla() { };

	int32 Gold{ 0 };

	EPorkType PorkType{ EPorkType::EPT_Normal };

	UPROPERTY(EditAnyWhere, Category = "Aiming")
	int32 balls{ 18 };

	UPROPERTY(EditAnyWhere, Category = "Aiming")
	float CadenzaMax { 0.15f };

	UPROPERTY(EditAnyWhere, Category = "Aiming")
	float Moltiplicatore{ 1.8f  };

	float Cadenza{ 0.2 };

public:

	bool enabled{ false };

	UPROPERTY(BlueprintReadWrite)
	bool hasgoal{ false };
	//UStaticMeshComponent* GetBody() { }
	class APigMode* PGM;

	UFUNCTION(BlueprintPure)
	FORCEINLINE EPorkType GetPorkType() { return PorkType; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPorkType(EPorkType NewState) { PorkType = NewState; };

	void AddGold();

	FORCEINLINE void AddPower() { MaxPower += 100; };

	FORCEINLINE void SetGold(int32 G) { Gold = G; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetGold() { return Gold; };

	class APigPawn* Maiale;

	// get per aver accesso alla mesh dal proiettile
	FORCEINLINE UStaticMeshComponent* GetBody() { return Body; };

	//Sets default values for this pawn's properties
	ACannonPawn();

	UFUNCTION(BlueprintPure)// BPPure per funzioni che non hanno eseguibile
	float GetPercent() { return Power / MaxPower; };
	
	UFUNCTION(BlueprintPure)
	float GetPitchRotation();

	UFUNCTION(BlueprintPure)
	float GetYawRotation() { return int32(GetActorRotation().Yaw); };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void FirePress();

	void FireRelease();

	void ChangePawn();

	void MoveForward(float value);

	void MoveRight(float value);

	void LookUp(float value);

	void Turn(float value);
};
