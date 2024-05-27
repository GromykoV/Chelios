// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/FireWeapon/FireWeapon.h"
#include "HitScanWeapon.generated.h"

#define TRACE_LENGTH 50000.f

/**
 * 
 */
UCLASS()
class CHELIOS_API AHitScanWeapon : public AFireWeapon
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void Fire(const FVector& HitTarget) override;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	float Damage = 40.f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* HitSound;

private:
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;

};
