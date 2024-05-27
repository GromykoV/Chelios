// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CheliosTypes/CombatState.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class CHELIOS_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
//======================================================================
// FUNCTIONS
//======================================================================

	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================
	UPROPERTY(BlueprintReadOnly)
	EIdlePose IdlePose = EIdlePose::EIP_Idle1;

protected:
//======================================================================
// FUNCTIONS
//======================================================================
	virtual void BeginPlay() override;

	/** <ABaseCharacter> */
	virtual void Die() override;
	void SpawnXP();
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
//======================================================================
// FUNCTIONS
//======================================================================

	/** AI Behavior */
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEdgedWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	int32 XPAmount = 1000;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 50.f;
	
	UPROPERTY()
	class ACheliosCharacter* Character;

	UPROPERTY()
	class AAIController* EnemyController;

	// Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	/** Combat */
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 180.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.3f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 0.8f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 350.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AXPPickup> BuffClass;
};
