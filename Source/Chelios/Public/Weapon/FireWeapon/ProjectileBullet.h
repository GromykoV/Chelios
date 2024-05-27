// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/FireWeapon/Projectile.h"
#include "ProjectileBullet.generated.h"


/**
 * 
 */
UCLASS()
class CHELIOS_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	AProjectileBullet();

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
