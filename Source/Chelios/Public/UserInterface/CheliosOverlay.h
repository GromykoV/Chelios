// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheliosOverlay.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class CHELIOS_API UCheliosOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	void SetStaminaBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetXP(int32 XP);
	void SetLevel(int32 Level);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldCoinText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;

	UPROPERTY()
	class UAttributeComponent* Attributes;

};
