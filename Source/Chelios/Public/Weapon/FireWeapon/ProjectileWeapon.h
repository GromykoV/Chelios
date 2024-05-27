// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/FireWeapon/FireWeapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API AProjectileWeapon : public AFireWeapon
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
};
