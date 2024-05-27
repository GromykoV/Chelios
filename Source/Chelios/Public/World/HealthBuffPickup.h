// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/BuffPickup.h"
#include "HealthBuffPickup.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API AHealthBuffPickup : public ABuffPickup
{
	GENERATED_BODY()

public:
//======================================================================
// FUNCTIONS
//======================================================================

	AHealthBuffPickup();

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
	float HealAmount = 100.f;

	UPROPERTY(EditAnywhere)
	float HealingTime = 5.f;

};
