// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/BuffPickup.h"
#include "Character/CheliosTypes/WeaponTypes.h"
#include "AmmoBuffPickup.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API AAmmoBuffPickup : public ABuffPickup
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
	int32 AmmoAmount = 30;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};
