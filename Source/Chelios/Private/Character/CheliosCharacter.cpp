// Portfolio by Vadim Gromyko


// Character
#include "Character/CheliosCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/CheliosAnimInstance.h"
#include "Character/CheliosPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Chelios/CheliosGameModeBase.h"
#include "CheliosComponents/AttributeComponent.h"
#include "UserInterface/CheliosOverlay.h"

// Inventory
#include "CheliosComponents/InventoryComponent.h"
#include "UserInterface/CheliosHUD.h"
#include "World/Pickup.h"

// Weapon
#include "Weapon/FireWeapon/FireWeapon.h"
#include "Weapon/EdgedWeapon/EdgedWeapon.h"
#include "Character/CheliosTypes/WeaponTypes.h"
#include "CheliosComponents/CombatComponent.h"

// PickUps
#include "World/XPPickup.h"
#include "World/CoinPickup.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"


ACheliosCharacter::ACheliosCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom3PV = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom3PV"));
	CameraBoom3PV->SetupAttachment(GetMesh());
	CameraBoom3PV->TargetArmLength = 300.f;
	CameraBoom3PV->bUsePawnControlRotation = true;

	FollowCamera3PV = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera3PV"));
	FollowCamera3PV->SetupAttachment(CameraBoom3PV, USpringArmComponent::SocketName);
	FollowCamera3PV->bUsePawnControlRotation = false;

	FollowCamera1PV = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera1PV"));
	FollowCamera1PV->SetupAttachment(GetMesh(), "CameraSocket");
	FollowCamera1PV->bUsePawnControlRotation = true;
	FollowCamera1PV->SetFieldOfView(100.f);

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(32);
	PlayerInventory->SetWeightCapacity(50.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 250.0f;

}

void ACheliosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACheliosCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACheliosCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACheliosCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACheliosCharacter::LookUp);

	PlayerInputComponent->BindAction("ChangeView", IE_Pressed, this, &ACheliosCharacter::ViewChange);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACheliosCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACheliosCharacter::StopSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACheliosCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ACheliosCharacter::EquipButtonPressed);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACheliosCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACheliosCharacter::EndInteract);
	PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &ACheliosCharacter::ToggleMenu);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACheliosCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACheliosCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACheliosCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACheliosCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACheliosCharacter::ReloadButtonPressed);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &ACheliosCharacter::Drop);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACheliosCharacter::Dodge);
}

void ACheliosCharacter::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<ACheliosHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->AddCharacterOverlay();

	UpdateHUDAmmo();
	InitializeOverlay();
	UpdateHUDHealth();
	UpdateHUDShield();
	UpdateHUDXP();
	UpdateHUDGold();

	Tags.Add(FName("EngageableTarget"));
}

void ACheliosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);

	if (Attributes && CheliosOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		CheliosOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
	if (bIsSprinting)
	{
		Attributes->UseStamina(Attributes->GetStaminaSpendRate());
		CheliosOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ACheliosCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
}

//=============Character Movement=============

void ACheliosCharacter::MoveForward(float Value)
{
	if (Controller != nullptr || Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
		if (bIsSprinting && Value > 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 220.f;
		}
	}
}

void ACheliosCharacter::MoveRight(float Value)
{
	if (Controller != nullptr || Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ACheliosCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ACheliosCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ACheliosCharacter::Sprint()
{
	if (Attributes->GetStamina() > 1.f)
	{
		bIsSprinting = true;
	}
}

void ACheliosCharacter::StopSprint()
{
	bIsSprinting = false;
}

void ACheliosCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina() || GetCharacterMovement()->IsFalling()) return;
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		PlayDodgeMontage();
		ActionState = EActionState::EAS_Dodge;
		if (Attributes && CheliosOverlay)
		{
			Attributes->UseStamina(Attributes->GetDodgeCost());
			CheliosOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		}
	}
}

void ACheliosCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}

void ACheliosCharacter::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

//=============Character Combat=============

void ACheliosCharacter::AimButtonPressed()
{
	if (Combat && Combat->EquippedWeapon && !bIsDead)
	{
		Combat->SetAiming(true);
		FollowCamera1PV->SetFieldOfView(50.f);
	}
}

void ACheliosCharacter::AimButtonReleased()
{
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->SetAiming(false);
		FollowCamera1PV->SetFieldOfView(100.f);
	}
}

void ACheliosCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		StartingAimRotation = { 0.0f, 90.0f, 0.0f };
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		bUseControllerRotationYaw = true;
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;

	}

	AO_Pitch = GetBaseAimRotation().Pitch;
}

void ACheliosCharacter::FireButtonPressed()
{
	if (Combat && Combat->EquippedWeapon && ActionState == EActionState::EAS_Unoccupied)
	{
		Combat->FireButtonPressed(true);
	}
	if (IsWeaponEdgedEquipped())
	{
		Attack();
	}
}

void ACheliosCharacter::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
		ActionState = EActionState::EAS_Unoccupied;
	}
}

void ACheliosCharacter::ReloadButtonPressed()
{
	if (Combat && ActionState == EActionState::EAS_Unoccupied)
	{
		Combat->Reload();
	}
}

void ACheliosCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		if (Attributes && CheliosOverlay)
		{
			Attributes->UseStamina(10.f);
			CheliosOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		}
		ActionState = EActionState::EAS_Attacking;
	}
}

void ACheliosCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ACheliosCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& Attributes->GetStamina() > 10.f;
}

void ACheliosCharacter::Die()
{
	Tags.Add(FName("Dead"));

	ActionState = EActionState::EAS_Dead;

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::QueryAndPhysics));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	DeathWidget();
	bIsDead = true;

	if (Combat && Combat->EquippedWeapon)
	{
		GetEquippedWeapon()->Dropped();
	}
	if (Combat && Combat->EquippedEdgedWeapon)
	{
		GetEquippedEdgedWeapon()->Dropped();
	}
	DisableInput(CheliosPlayerController);

	FollowCamera3PV->SetActive(true);
	FollowCamera1PV->SetActive(false);
	bUseControllerRotationYaw = false;
	bViewChange = true;
	/*GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ACheliosCharacter::ElimTimerFinished,
		ElimDelay
	);
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, 1.f, false);*/

}

float ACheliosCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	UpdateHUDHealth();
	UpdateHUDShield();

	return DamageAmount;
}

void ACheliosCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ACheliosCharacter::ViewChange()
{
	if (bViewChange && !bIsDead)
	{
		FollowCamera3PV->SetActive(false);
		FollowCamera1PV->SetActive(true);
		bUseControllerRotationYaw = true;
		bViewChange = false;
	}
	else
	{
		FollowCamera3PV->SetActive(true);
		FollowCamera1PV->SetActive(false);
		bUseControllerRotationYaw = false;
		bViewChange = true;
	}
}

void ACheliosCharacter::EquipButtonPressed()
{
	if (Combat && !OverlappingEdgedWeapon)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
	if (Combat && !OverlappingWeapon)
	{
		Combat->EquipEdgedWeapon(OverlappingEdgedWeapon);
	}
}

bool ACheliosCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ACheliosCharacter::IsWeaponEdgedEquipped()
{
	return (Combat && Combat->EquippedEdgedWeapon);
}

AFireWeapon* ACheliosCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

AEdgedWeapon* ACheliosCharacter::GetEquippedEdgedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedEdgedWeapon;
}

void ACheliosCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ACheliosCharacter::PlayReloadMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (Combat->EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_RocketLauncher:
			SectionName = FName("Rocket");
			break;
		case EWeaponType::EWT_Pistol:
			SectionName = FName("Pistol");
			break;
		case EWeaponType::EWT_Shotgun:
			SectionName = FName("Shotgun");
			break;
		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("SniperRifle");
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ACheliosCharacter::Drop()
{
	if (Combat && Combat->EquippedWeapon && ActionState == EActionState::EAS_Unoccupied)
	{
		Combat->EquippedWeapon->Dropped();
		Combat->EquippedWeapon->SetWeaponState(EFireWeaponState::EFWS_Initial);
		Combat->EquippedWeapon = nullptr;
		if (bViewChange)
		{
			bUseControllerRotationYaw = false;
		}
	}
	if (Combat && Combat->EquippedEdgedWeapon)
	{
		Combat->EquippedEdgedWeapon->Dropped();
		Combat->EquippedEdgedWeapon->SetWeaponState(EEdgedWeaponState::EEWS_Initial);
		Combat->EquippedEdgedWeapon = nullptr;
		if (bViewChange)
		{
			bUseControllerRotationYaw = false;
		}
	}
	ActionState = EActionState::EAS_Unoccupied;
}

bool ACheliosCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

void ACheliosCharacter::SetOverlappingWeapon(AFireWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ACheliosCharacter::SetOverlappingEdgedWeapon(AEdgedWeapon* Weapon)
{
	if (OverlappingEdgedWeapon)
	{
		OverlappingEdgedWeapon->ShowPickupWidget(false);
	}
	OverlappingEdgedWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingEdgedWeapon)
		{
			OverlappingEdgedWeapon->ShowPickupWidget(true);
		}
	}
}

EActionState ACheliosCharacter::GetCombatActionState() const
{
	if (Combat == nullptr) return EActionState::EAS_MAX;
	return Combat->ActionState;
}

void ACheliosCharacter::ElimTimerFinished()
{

}

void ACheliosCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

//=============Character Overlay=============

void ACheliosCharacter::AddXP(AXPPickup* XP)
{
	if (Attributes && CheliosOverlay)
	{
		Attributes->AddXP(XP->GetXP());
		CheliosOverlay->SetXP(Attributes->GetGold());
		UpdateHUDXP();
	}
}

void ACheliosCharacter::AddGold(ACoinPickup* Coin)
{
	if (Attributes && CheliosOverlay)
	{
		Attributes->AddGold(Coin->GetGold());
		CheliosOverlay->SetGold(Attributes->GetGold());
		UpdateHUDGold();
	}
}

void ACheliosCharacter::HealRampUp(float DeltaTime)
{
	bool bIsHealing = bHealing;
	if (!bIsHealing || this == nullptr) return;

	const float HealThisFrame = HealingRate * DeltaTime;
	Attributes->SetHealth(FMath::Clamp(Attributes->GetHealth() + HealThisFrame, 0.f, Attributes->GetMaxHealth()));
	UpdateHUDHealth();
	AmountToHeal -= HealThisFrame;

	if (AmountToHeal <= 0.f || Attributes->GetHealth() >= Attributes->GetMaxHealth())
	{
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

void ACheliosCharacter::ShieldRampUp(float DeltaTime)
{
	if (!bReplenishingShield || this == nullptr) return;

	const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;
	Attributes->SetShield(FMath::Clamp(Attributes->GetShield() + ReplenishThisFrame, 0.f, Attributes->GetMaxShield()));
	UpdateHUDShield();
	ShieldReplenishAmount -= ReplenishThisFrame;

	if (ShieldReplenishAmount <= 0.f || Attributes->GetShield() >= Attributes->GetMaxShield())
	{
		bReplenishingShield = false;
		ShieldReplenishAmount = 0.f;
	}
}

void ACheliosCharacter::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
}

void ACheliosCharacter::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	bReplenishingShield = true;
	ShieldReplenishRate = ShieldAmount / ReplenishTime;
	ShieldReplenishAmount += ShieldAmount;
}

bool ACheliosCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

void ACheliosCharacter::InitializeOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ACheliosHUD* CheliosHUD = Cast<ACheliosHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (CheliosHUD)
		{
			CheliosOverlay = CheliosHUD->CharacterOverlay;
			if (CheliosOverlay && Attributes)
			{
				Attributes->SetHealth(100.f);
				Attributes->SetShield(20.f);
				CheliosOverlay->SetStaminaBarPercent(1.f);
				CheliosOverlay->SetGold(0);
				Attributes->SetXP(0);
				CheliosOverlay->SetLevel(1);
			}
		}
	}
}

void ACheliosCharacter::UpdateHUDHealth()
{
	CheliosPlayerController = Cast<ACheliosPlayerController>(Controller);
	if (CheliosPlayerController)
	{
		CheliosPlayerController->SetHUDHealth(Attributes->GetHealth(), Attributes->GetMaxHealth());
	}
}

void ACheliosCharacter::UpdateHUDShield()
{
	CheliosPlayerController = CheliosPlayerController == nullptr ? Cast<ACheliosPlayerController>(Controller) : CheliosPlayerController;
	if (CheliosPlayerController)
	{
		CheliosPlayerController->SetHUDShield(Attributes->GetShield(), Attributes->GetMaxShield());
	}
}

void ACheliosCharacter::UpdateHUDXP()
{
	CheliosPlayerController = CheliosPlayerController == nullptr ? Cast<ACheliosPlayerController>(Controller) : CheliosPlayerController;
	if (CheliosPlayerController)
	{
		CheliosPlayerController->SetHUDXP(Attributes->GetXP());
	}
}

void ACheliosCharacter::UpdateHUDGold()
{
	CheliosPlayerController = CheliosPlayerController == nullptr ? Cast<ACheliosPlayerController>(Controller) : CheliosPlayerController;
	if (CheliosPlayerController)
	{
		CheliosPlayerController->SetHUDGold(Attributes->GetGold());
	}
}

bool ACheliosCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

bool ACheliosCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ACheliosCharacter::UpdateHUDAmmo()
{
	CheliosPlayerController = CheliosPlayerController == nullptr ? Cast<ACheliosPlayerController>(Controller) : CheliosPlayerController;
	if (CheliosPlayerController && Combat && Combat->EquippedWeapon)
	{
		CheliosPlayerController->SetHUDCarriedAmmo(Combat->CarriedAmmo);
		CheliosPlayerController->SetHUDWeaponAmmo(Combat->EquippedWeapon->GetAmmo());
	}
}

void ACheliosCharacter::Vault(float Stamina)
{
	if (Attributes && CheliosOverlay)
	{
		Attributes->UseStamina(Stamina);
		CheliosOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

//=============Character Inventory=============

void ACheliosCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ FollowCamera1PV->GetComponentLocation() }; // *GetPawnViewLocation()
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	if (!bViewChange)
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
		NoInteractableFound();
	}
}

void ACheliosCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void ACheliosCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}
	}
	HUD->HideInteractionWidget();

	InteractionData.CurrentInteractable = nullptr;
	TargetInteractable = nullptr;

}

void ACheliosCharacter::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&ACheliosCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void ACheliosCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ACheliosCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
	UWorld* World = GetWorld();
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			World,
			PickUpSound,
			GetActorLocation()
		);
	}
}

void ACheliosCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

void ACheliosCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void ACheliosCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}
