// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuffPickup.generated.h"

class UNiagaraComponent;

UCLASS()
class CHELIOS_API ABuffPickup : public AActor
{
	GENERATED_BODY()
	
public:	
//======================================================================
// FUNCTIONS
//======================================================================

	ABuffPickup();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:	
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* PickupEffectComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

};
