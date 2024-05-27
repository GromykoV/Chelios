// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CoinPickup.h"
#include "Interfaces/PickupInterface.h"
#include "CheliosComponents/AttributeComponent.h"


ACoinPickup::ACoinPickup()
{
}

void ACoinPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddGold(this);
		SpawnPickupSound();
	}

	Destroy();
}
