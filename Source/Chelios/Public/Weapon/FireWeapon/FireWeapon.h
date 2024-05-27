// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/CheliosTypes/WeaponTypes.h"
#include "FireWeapon.generated.h"

class UWidgetComponent;

UENUM(BlueprintType)
enum class EFireWeaponState : uint8
{
	EFWS_Initial UMETA(DisplayName = "Initial State"),
	EFWS_Equipped UMETA(DisplayName = "Equipped"),
	EFWS_Dropped UMETA(DisplayName = "Dropped"),

	EFWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class CHELIOS_API AFireWeapon : public AActor
{
	GENERATED_BODY()

public:
//======================================================================
// FUNCTIONS
//======================================================================

	AFireWeapon();
	friend class ACheliosCharacter;

	virtual void Tick(float DeltaTime) override;
	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);
	void Dropped();
	void SetWeaponState(EFireWeaponState State);

	void SetHUDAmmo();

	void SpendRound();
	void AddAmmo(int32 AmmoToAdd);
	bool IsEmpty();
	bool IsFull();
	void Reloading();

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================
	/**
	* Textures for the weapon crosshairs
	*/

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;

	/**
	* Automatic fire
	*/
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Weapon Properties")
	USkeletalMeshComponent* FireWeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EFireWeaponState FireWeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY()
	class ACheliosCharacter* CheliosOwnerCharacter;

	UPROPERTY()
	class ACheliosPlayerController* CheliosOwnerController;

public:

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return FireWeaponMesh; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }

};
