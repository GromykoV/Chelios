// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/FireWeapon/HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void Fire(const FVector& HitTarget) override;

private:
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	uint32 NumberOfPellets = 10;
};
