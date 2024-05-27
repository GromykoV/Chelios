// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FireWeapon/FireWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CheliosCharacter.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/FireWeapon/Casing.h"
#include "Character/CheliosPlayerController.h"
#include "Engine/SkeletalMeshSocket.h"


AFireWeapon::AFireWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	FireWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireWeaponMesh"));
	SetRootComponent(FireWeaponMesh);

	FireWeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	FireWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	FireWeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	
}

void AFireWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireWeapon::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AFireWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AFireWeapon::OnSphereEndOverlap);

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}

void AFireWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACheliosCharacter* CheliosCharacter = Cast<ACheliosCharacter>(OtherActor);
	if (CheliosCharacter)
	{
		CheliosCharacter->SetOverlappingWeapon(this);
	}
}

void AFireWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACheliosCharacter* CheliosCharacter = Cast<ACheliosCharacter>(OtherActor);
	if (CheliosCharacter && PickupWidget)
	{
		CheliosCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AFireWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AFireWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		FireWeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if (CasingClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = FireWeaponMesh->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(FireWeaponMesh);

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ACasing>(
					CasingClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
				);
			}
		}
	}
	SpendRound();
}

void AFireWeapon::Dropped()
{
	SetWeaponState(EFireWeaponState::EFWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	FireWeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	CheliosOwnerCharacter = nullptr;
	CheliosOwnerController = nullptr;
}

void AFireWeapon::SetWeaponState(EFireWeaponState State)
{
	FireWeaponState = State;
	switch (FireWeaponState)
	{
	case EFireWeaponState::EFWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FireWeaponMesh->SetSimulatePhysics(false);
		FireWeaponMesh->SetEnableGravity(false);
		FireWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EFireWeaponState::EFWS_Dropped:
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		FireWeaponMesh->SetSimulatePhysics(true);
		FireWeaponMesh->SetEnableGravity(true);
		FireWeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FireWeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		FireWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		FireWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		break;
	}
}

void AFireWeapon::SetHUDAmmo()
{
	CheliosOwnerCharacter = CheliosOwnerCharacter == nullptr ? Cast<ACheliosCharacter>(GetOwner()) : CheliosOwnerCharacter;
	if (CheliosOwnerCharacter)
	{
		CheliosOwnerController = CheliosOwnerController == nullptr ? Cast<ACheliosPlayerController>(CheliosOwnerCharacter->Controller) : CheliosOwnerController;
		if (CheliosOwnerController)
		{
			CheliosOwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

void AFireWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHUDAmmo();
}

void AFireWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	SetHUDAmmo();
}

bool AFireWeapon::IsEmpty()
{
	return Ammo <= 0;
}

bool AFireWeapon::IsFull()
{
	return Ammo == MagCapacity;
}

void AFireWeapon::Reloading()
{
	FireWeaponMesh->PlayAnimation(ReloadAnimation, false);
}
