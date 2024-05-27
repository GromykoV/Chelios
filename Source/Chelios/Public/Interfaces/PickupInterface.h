// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHELIOS_API IPickupInterface
{
	GENERATED_BODY()

public:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void SetOverlappingItem(class ABuffPickup* Buff);
	virtual void AddXP(class AXPPickup* XP);
	virtual void AddGold(class ACoinPickup* Coin);
};
