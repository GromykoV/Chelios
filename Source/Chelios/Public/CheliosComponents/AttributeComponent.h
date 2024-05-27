// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHELIOS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
//======================================================================
// FUNCTIONS
//======================================================================

	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetShieldPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddXP(int32 NumberOfXP);
	void AddGold(int32 AmountOfGold);

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;

private:
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	// Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	// Current Shield
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Shield;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxShield;

	// Current Stamina
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 XP;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 15.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 3.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaSpendRate = 0.2f;

public:

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE void SetXP(int32 Amount) { XP = Amount; }
	FORCEINLINE void SetGold(int32 Amount) { Gold = Amount; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }

	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }

	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetStaminaSpendRate() const { return StaminaSpendRate; }

};
