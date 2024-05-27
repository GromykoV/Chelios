// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	void SetHealthPercent(float Percent);

private:
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
