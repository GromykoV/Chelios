// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/BuffPickup.h"
#include "CoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API ACoinPickup : public ABuffPickup
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	ACoinPickup();

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY()
	class UAttributeComponent* Attributes;

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

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
