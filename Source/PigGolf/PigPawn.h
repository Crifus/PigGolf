// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PigPawn.generated.h"

UCLASS()
class PIGGOLF_API APigPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm{ nullptr };

	bool bHisLanded{ false };

	UFUNCTION()
	void MyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnyWhere, Category = "Sound")
	class USoundCue* GrugnitoBase;

	UPROPERTY(EditAnyWhere, Category = "Sound")
	USoundCue* GrugnitoSoffre;

	UPROPERTY(EditAnyWhere, Category = "Sound")
	USoundCue* Peto;

	UPROPERTY(EditAnyWhere, Category = "Sound")
	USoundCue* Splash;

	UPROPERTY(EditAnyWhere, Category = "Vfx")
	class UNiagaraSystem* Fart;


	UPROPERTY(EditAnyWhere, Category = "Vfx")
	UNiagaraSystem* BigFart;

	UPROPERTY(EditAnyWhere, Category = "Vfx")
	UNiagaraSystem* WaterVFX;

	UPROPERTY(EditAnyWhere, Category = "Peti")
	float Pow{ 10 };

	uint8 Peti{ 2 };

	UPROPERTY(EditAnyWhere, Category = "Peti")
	uint8 MaxPeti {	2 };

	class ACannonPawn* Cannone;

public:

	class APigMode* PGM;

	bool bBouncing{ false };// diventa vero per pochi istanti quando il maiale rimbalza

	void ResetBounce() { bBouncing = false; }; // funzione di callback

	bool GetLanded() { return bHisLanded; };

	void AddFart() { if (Peti < MaxPeti) Peti++; };

	ACannonPawn* GetCannone() { return Cannone; };

	UFUNCTION(BlueprintPure)
	uint8 GetPeti() { return Peti; };

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	UStaticMeshComponent* MeshRef {nullptr};

	FORCEINLINE void SetCannone(ACannonPawn* Can) { Cannone = Can; };

	// Sets default values for this pawn's properties
	APigPawn();

	UPROPERTY(BlueprintReadWrite,VisibleAnyWhere)
	class USphereComponent* Sfera{ nullptr };

	void Inizializza();
    void Disattiva();

	void Water();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool VerifySphere(FVector NewLoc);

	void BlendCamera(AActor* BlendTo, float BlendTime = 1.f, bool Destroy = false);

	void ReturnCannon();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void FirePress();

	void AttachFart();

	void LookUp(float value);

	void Turn(float value);

};
