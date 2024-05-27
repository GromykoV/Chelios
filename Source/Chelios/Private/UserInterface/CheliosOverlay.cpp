// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/CheliosOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CheliosComponents/AttributeComponent.h"


void UCheliosOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
		if (Percent < 0.25f)
		{
			StaminaBar->SetFillColorAndOpacity(FColor::Red);
		}
		else
		{
			StaminaBar->SetFillColorAndOpacity(FColor::Blue);
		}
	}
}

void UCheliosOverlay::SetGold(int32 Gold)
{
	if (GoldCoinText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String);
		GoldCoinText->SetText(Text);
	}
}

void UCheliosOverlay::SetXP(int32 XP)
{
	if (XPText)
	{
		const FString String = FString::Printf(TEXT("%d"), XP);
		const FText Text = FText::FromString(String);
		XPText->SetText(Text);
	}
}

void UCheliosOverlay::SetLevel(int32 Level)
{
	if (LevelText)
	{
		const FString String = FString::Printf(TEXT("%d"), Level);
		const FText Text = FText::FromString(String);
		LevelText->SetText(Text);
	}
}
