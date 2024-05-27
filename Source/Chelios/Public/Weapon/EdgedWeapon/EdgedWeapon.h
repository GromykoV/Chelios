// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/CheliosTypes/WeaponTypes.h"
#include "EdgedWeapon.generated.h"

class USoundBase;
class UBoxComponent;

UENUM(BlueprintType)
enum class EEdgedWeaponState : uint8
{
	EEWS_Initial UMETA(DisplayName = "Initial State"),
	EEWS_Equipped UMETA(DisplayName = "Equipped"),
	EEWS_Dropped UMETA(DisplayName = "Dropped"),

	EEWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class CHELIOS_API AEdgedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
//======================================================================
// FUNCTIONS
//======================================================================

	AEdgedWeapon();
	friend class ACheliosCharacter;

	void DisableSphereCollision();
	void DiactivateEmbers();
	virtual void Tick(float DeltaTime) override;

	void SetWeaponState(EEdgedWeaponState State);
	void Dropped();
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	TArray<AActor*> IgnoreActors;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

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

	virtual void BeginPlay() override;

	void ShowPickupWidget(bool bShowWidget);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* EmbersEffect;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
	UStaticMeshComponent* EdgedWeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
	EEdgedWeaponState EdgedWeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* PickupWidget;

	UPROPERTY()
	class ACheliosCharacter* CheliosOwnerCharacter;

	UPROPERTY()
	class ACheliosPlayerController* CheliosOwnerController;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 30.f;
private:
//======================================================================
// FUNCTIONS
//======================================================================

	void BoxTrace(FHitResult& BoxHit);

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

public:

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }

	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return EdgedWeaponMesh; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

};
