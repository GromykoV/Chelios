// Fill out your copyright notice in the Description page of Project Settings.


#include "World/HealthBuffPickup.h"
#include "Character/CheliosCharacter.h"


AHealthBuffPickup::AHealthBuffPickup()
{
	
}

void AHealthBuffPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACheliosCharacter* CheliosCharacter = Cast<ACheliosCharacter>(OtherActor);
	if (CheliosCharacter)
	{
		CheliosCharacter->Heal(HealAmount, HealingTime);
	}

	Destroy();
}
