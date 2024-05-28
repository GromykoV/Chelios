// Portfolio by Vadim Gromyko

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/PickupInterface.h"
#include "Character/CheliosTypes/CombatState.h"
#include "CheliosCharacter.generated.h"

class ACheliosHUD;
class UInventoryComponent;
class UItemBase;
class AFireWeapon;
class UWidgetComponent;
class AEdgedWeapon;
class UCheliosOverlay;
class AXPPickup;
class ACoinPickup;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS()
class CHELIOS_API ACheliosCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	//======================================================================
	// FUNCTIONS
	//======================================================================
	ACheliosCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void AddXP(class AXPPickup* XP) override;
	virtual void AddGold(ACoinPickup* Coin) override;

	void UpdateInteractionWidget() const;
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
	void SetOverlappingWeapon(AFireWeapon* Weapon);
	void SetOverlappingEdgedWeapon(AEdgedWeapon* Weapon);

	virtual void PostInitializeComponents() override;

	bool IsWeaponEquipped();
	bool IsWeaponEdgedEquipped();
	bool IsAiming();
	void Drop();
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();

	UFUNCTION(BlueprintImplementableEvent)
	void DeathWidget();

	void UpdateHUDAmmo();
	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDXP();
	void UpdateHUDGold();

	UFUNCTION(BlueprintCallable)
	void Vault(float Stamina);

	void Heal(float HealAmount, float HealingTime);	// Buff Health
	void ReplenishShield(float ShieldAmount, float ReplenishTime);	// Buff Shield

	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom3PV;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera3PV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* FollowCamera1PV;

	UPROPERTY(BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bViewChange = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	AFireWeapon* GetEquippedWeapon();
	AEdgedWeapon* GetEquippedEdgedWeapon();

	EActionState GetCombatActionState() const;

	class ACheliosPlayerController* CheliosPlayerController;

protected:
	//======================================================================
	// FUNCTIONS
	//======================================================================
	virtual void BeginPlay() override;

	void HealRampUp(float DeltaTime); // Buff Health
	void ShieldRampUp(float DeltaTime); // Buff Shield

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Sprint();
	void StopSprint();
	void CrouchButtonPressed();
	void ViewChange();

	void EquipButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime);

	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButtonPressed();
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void Die() override;

	bool HasEnoughStamina();
	bool IsOccupied();

	void Dodge();
	virtual void DodgeEnd() override;

	bool IsUnoccupied();
	void InitializeOverlay();

	void ElimTimerFinished();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	void ToggleMenu();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UPROPERTY()
	ACheliosHUD* HUD;

	UPROPERTY()
	UCheliosOverlay* CheliosOverlay;

	UPROPERTY()
	AFireWeapon* OverlappingWeapon;

	UPROPERTY()
	AEdgedWeapon* OverlappingEdgedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 5.f;

	bool bHealing = false;
	float HealingRate = 0;
	float AmountToHeal = 0.f;

	bool bReplenishingShield = false;
	float ShieldReplenishRate = 0.f;
	float ShieldReplenishAmount = 0.f;

private:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	class USoundCue* PickUpSound;

public:

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }

	FORCEINLINE bool IsDead() const { return bIsDead; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
