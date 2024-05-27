// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/BuffPickup.h"
#include "ShieldBuffPickup.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API AShieldBuffPickup : public ABuffPickup
{
	GENERATED_BODY()
protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	float ShieldReplenishAmount = 100.f;

	UPROPERTY(EditAnywhere)
	float ShieldReplenishTime = 5.f;
};
