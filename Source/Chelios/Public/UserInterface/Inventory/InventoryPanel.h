// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class ACheliosCharacter;
class UWrapBox;
class UTextBlock;
class UInventoryComponent;
class UInventoryItemSlot;

/**
 * 
 */
UCLASS()
class CHELIOS_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
public:
//======================================================================
// FUNCTIONS
//======================================================================

	UFUNCTION()
	void RefreshInventory();

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	ACheliosCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
