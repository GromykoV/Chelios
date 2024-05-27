// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CheliosPlayerController.h"
#include "UserInterface/CheliosHUD.h"
#include "UserInterface/CheliosOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/CheliosCharacter.h"
#include "Components/Image.h"
#include "Weapon/FireWeapon/FireWeapon.h"
#include "CheliosComponents/AttributeComponent.h"
#include "CheliosComponents/CombatComponent.h"


void ACheliosPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CheliosHUD = Cast<ACheliosHUD>(GetHUD());
}

void ACheliosPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (CheliosHUD && CheliosHUD->CharacterOverlay)
		{
			CharacterOverlay = CheliosHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeShield) SetHUDShield(HUDShield, HUDMaxShield);
			}
		}
	}
}

void ACheliosPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	CheliosHUD = CheliosHUD == nullptr ? Cast<ACheliosHUD>(GetHUD()) : CheliosHUD;
	bool bHUDValid = CheliosHUD &&
		CheliosHUD->CharacterOverlay &&
		CheliosHUD->CharacterOverlay->WeaponAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		CheliosHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

void ACheliosPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	CheliosHUD = CheliosHUD == nullptr ? Cast<ACheliosHUD>(GetHUD()) : CheliosHUD;
	bool bHUDValid = CheliosHUD &&
		CheliosHUD->CharacterOverlay &&
		CheliosHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		CheliosHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitializeCarriedAmmo = true;
		HUDCarriedAmmo = Ammo;
	}
}

void ACheliosPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	CheliosHUD = CheliosHUD == nullptr ? Cast<ACheliosHUD>(GetHUD()) : CheliosHUD;

	bool bHUDValid = CheliosHUD &&
		CheliosHUD->CharacterOverlay &&
		CheliosHUD->CharacterOverlay->HealthBar &&
		CheliosHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		CheliosHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		if (HealthPercent < 0.2f)
		{
			CheliosHUD->CharacterOverlay->HealthBar->SetFillColorAndOpacity(FColor::White);
		}
		else
		{
			CheliosHUD->CharacterOverlay->HealthBar->SetFillColorAndOpacity(FColor::Red);
		}
		FString HealthText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Health));
		CheliosHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeHealth = false;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void ACheliosPlayerController::SetHUDShield(float Shield, float MaxShield)
{
	CheliosHUD = CheliosHUD == nullptr ? Cast<ACheliosHUD>(GetHUD()) : CheliosHUD;
	bool bHUDValid = CheliosHUD &&
		CheliosHUD->CharacterOverlay &&
		CheliosHUD->CharacterOverlay->ShieldBar &&
		CheliosHUD->CharacterOverlay->ShieldText;
	if (bHUDValid)
	{
		const float ShieldPercent = Shield / MaxShield;
		CheliosHUD->CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		FString ShieldText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Shield));
		CheliosHUD->CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
	}
	else
	{
		bInitializeShield = false;
		HUDShield = Shield;
		HUDMaxShield = MaxShield;
	}
}

void ACheliosPlayerController::SetHUDXP(int32 XP)
{
	CheliosHUD = CheliosHUD == nullptr ? Cast<ACheliosHUD>(GetHUD()) : CheliosHUD;
	bool bHUDValid = CheliosHUD &&
		CheliosHUD->CharacterOverlay &&
		CheliosHUD->CharacterOverlay->XPText;
	if (bHUDValid)
	{
		const FString String = FString::Printf(TEXT("%d"), XP);
		const FText Text = FText::FromString(String);
		CheliosHUD->CharacterOverlay->XPText->SetText(Text);
	}
	else
	{
		bInitializeXP = false;
		HUDXP = XP;
	}
}

void ACheliosPlayerController::SetHUDGold(int32 Gold)
{
	CheliosHUD = CheliosHUD == nullptr ? Cast<ACheliosHUD>(GetHUD()) : CheliosHUD;
	bool bHUDValid = CheliosHUD &&
		CheliosHUD->CharacterOverlay &&
		CheliosHUD->CharacterOverlay->GoldCoinText;
	if (bHUDValid)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String);
		CheliosHUD->CharacterOverlay->GoldCoinText->SetText(Text);
	}
	else
	{
		bInitializeGold = false;
		HUDGold = Gold;
	}
}
