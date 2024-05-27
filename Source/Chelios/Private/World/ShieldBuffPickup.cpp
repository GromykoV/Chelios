// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ShieldBuffPickup.h"
#include "Character/CheliosCharacter.h"


void AShieldBuffPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACheliosCharacter* CheliosCharacter = Cast<ACheliosCharacter>(OtherActor);
	if (CheliosCharacter)
	{
		CheliosCharacter->ReplenishShield(ShieldReplenishAmount, ShieldReplenishTime);
	}

	Destroy();
}
