// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FireWeapon/Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Character/CheliosCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include <Interfaces/HitInterface.h>


void AShotgun::Fire(const FVector& HitTarget)
{
	AFireWeapon::Fire(HitTarget);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		TMap<ACheliosCharacter*, uint32> HitMap;
		for (uint32 i = 0; i < NumberOfPellets; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ACheliosCharacter* CheliosCharacter = Cast<ACheliosCharacter>(FireHit.GetActor());
			if (CheliosCharacter && InstigatorController)
			{
				if (HitMap.Contains(CheliosCharacter))
				{
					HitMap[CheliosCharacter]++;
				}
				else
				{
					HitMap.Emplace(CheliosCharacter, 1);
				}
			}

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}

			IHitInterface* HitInterface = Cast<IHitInterface>(FireHit.GetActor());
			if (HitInterface)
			{
				HitInterface->Execute_GetHit(FireHit.GetActor(), FireHit.ImpactPoint, GetOwner());
			}
			CreateFields(FireHit.ImpactPoint);

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-.5f, .5f)
				);
			}
		}
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					HitPair.Key,
					Damage * HitPair.Value,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}

		}
	}
}
