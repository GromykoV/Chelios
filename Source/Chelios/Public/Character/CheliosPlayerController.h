// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CheliosCharacter.h"

#include "CheliosPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHELIOS_API ACheliosPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDXP(int32 XP);
	void SetHUDGold(int32 Gold);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY()
	class UAttributeComponent* Attributes;

	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;

	void PollInit();

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY()
	class ACheliosHUD* CheliosHUD;

	UPROPERTY()
	class UCheliosOverlay* CharacterOverlay;

	float HUDHealth;
	bool bInitializeHealth = false;
	float HUDMaxHealth;
	float HUDShield;
	bool bInitializeShield = false;
	float HUDMaxShield;
	int32 HUDXP;
	bool bInitializeXP = false;
	int32 HUDGold;
	bool bInitializeGold = false;
	float HUDStamina;
	bool bInitializeStamina = false;
	float HUDMaxStamina;

};
