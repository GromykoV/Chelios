// Portfolio by Vadim Gromyko


#include "Character/CheliosAnimInstance.h"
#include "Character/CheliosCharacter.h"
#include "CheliosComponents/CombatComponent.h"
#include "Weapon/FireWeapon/FireWeapon.h"
#include "Weapon/EdgedWeapon/EdgedWeapon.h"
#include "Character/CheliosTypes/CombatState.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCheliosAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CheliosCharacter = Cast<ACheliosCharacter>(TryGetPawnOwner());
}

void UCheliosAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CheliosCharacter == nullptr)
	{
		CheliosCharacter = Cast<ACheliosCharacter>(TryGetPawnOwner());
	}
	if (CheliosCharacter == nullptr) return;

	FVector Velocity = CheliosCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	Direction = CalculateDirection(Velocity, CheliosCharacter->GetActorRotation());

	bIsCrouched = CheliosCharacter->bIsCrouched;
	bIsInAir = CheliosCharacter->GetCharacterMovement()->IsFalling();
	bWeaponEquipped = CheliosCharacter->IsWeaponEquipped();
	bEdgedWeaponEquipped = CheliosCharacter->IsWeaponEdgedEquipped();
	EquippedWeapon = CheliosCharacter->GetEquippedWeapon();
	EquippedEdgedWeapon = CheliosCharacter->GetEquippedEdgedWeapon();
	bAiming = CheliosCharacter->IsAiming();
	bDead = CheliosCharacter->IsDead();

	AO_Yaw = CheliosCharacter->GetAO_Yaw();
	AO_Pitch = CheliosCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && CheliosCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		CheliosCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}

	if (CheliosCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
	{
		bIsAccelerating = true;
	}
	else
	{
		bIsAccelerating = false;
	}

	ActionState = CheliosCharacter->GetActionState();
	DeathPose = CheliosCharacter->GetDeathPose();
}
