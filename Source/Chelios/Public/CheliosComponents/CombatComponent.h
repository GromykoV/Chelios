// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/CheliosTypes/CombatState.h"
#include "Character/CheliosTypes/WeaponTypes.h"
#include "UserInterface/CheliosHUD.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 50000.f

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHELIOS_API UCombatComponent : public UActorComponent
{	
	GENERATED_BODY()

public:
//======================================================================
// FUNCTIONS
//======================================================================

	UCombatComponent();
	friend class ACheliosCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void EquipWeapon(class AFireWeapon* FireWeaponToEquip);

	UFUNCTION()
	void EquipEdgedWeapon(class AEdgedWeapon* EdgedWeaponToEquip);

	UFUNCTION()
	void SetAiming(bool bIsAiming);

	void FireButtonPressed(bool bPressed);
	void Fire();
	void Attack();
	void Reload();

	void StartFireTimer();
	void FireTimerFinished();
	bool CanFire();
	void HandleReload();
	int32 AmountToReload();

	void UpdateCarriedAmmo();
	void UpdateAmmoValues();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	/**
	* Automatic fire
	*/

	FTimerHandle FireTimer;
	bool bCanFire = true;

	UPROPERTY()
	int32 CarriedAmmo;

	UPROPERTY()
	AFireWeapon* EquippedWeapon;

	UPROPERTY()
	AEdgedWeapon* EquippedEdgedWeapon;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	void InitializeCarriedAmmo();

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY()
	ACheliosCharacter* Character;

	UPROPERTY()
	class ACheliosPlayerController* Controller;

	UPROPERTY()
	class ACheliosHUD* HUD;

	UPROPERTY()
	class AEnemy* Enemy;

	UPROPERTY()
	bool bAiming;

	UPROPERTY()
	bool bFireButtonPressed;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

	UPROPERTY(EditAnywhere)
	int32 StartingAKAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 8;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 24;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 14;

	UPROPERTY()
	EActionState ActionState = EActionState::EAS_Unoccupied;
};
