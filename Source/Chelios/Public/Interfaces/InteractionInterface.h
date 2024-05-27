// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ACheliosCharacter;

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	Device UMETA(DisplayName = "Device"), // Doors, Windows
	Toggle UMETA(DisplayName = "Toggle"), // Buttons, Switches
	Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
		InteractableType(EInteractableType::Pickup),
		Name(FText::GetEmpty()),
		Action(FText::GetEmpty()),
		Quantity(0),
		InteractionDuration(0.0f)
	{

	};

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// Used only for pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity; // In blueprints only int32

	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHELIOS_API IInteractionInterface
{
	GENERATED_BODY()

public:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginFocus();
	virtual void EndFocus();
	virtual	void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(ACheliosCharacter* PlayerCharacter);

	FInteractableData InteractableData;

};
