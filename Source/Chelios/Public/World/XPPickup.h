// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/BuffPickup.h"
#include "XPPickup.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API AXPPickup : public ABuffPickup
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void Tick(float DeltaTime) override;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere, Category = "XP Properties")
	int32 XP;

	double DesiredZ;

	UPROPERTY(EditAnywhere)
	float DriftRate = -100.f;

public:

	FORCEINLINE int32 GetXP() const { return XP; }

	FORCEINLINE void SetXP(int32 NumberOfXP) { XP = NumberOfXP; }
};
