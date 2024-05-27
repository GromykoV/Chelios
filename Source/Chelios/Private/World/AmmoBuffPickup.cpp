// Fill out your copyright notice in the Description page of Project Settings.


#include "World/AmmoBuffPickup.h"
#include "Character/CheliosCharacter.h"
#include "CheliosComponents/CombatComponent.h"


void AAmmoBuffPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(
		OverlappedComponent,
		OtherActor,
		OtherComp,
		OtherBodyIndex,
		bFromSweep,
		SweepResult
	);

	ACheliosCharacter* CheliosCharacter = Cast<ACheliosCharacter>(OtherActor);
	if (CheliosCharacter)
	{
		UCombatComponent* Combat = CheliosCharacter->GetCombat();
		if (Combat)
		{
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	Destroy();
}
