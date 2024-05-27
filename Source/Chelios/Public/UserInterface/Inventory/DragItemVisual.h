// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

class UTextBlock;
class UBorder;
class UImage;

/**
 * 
 */
UCLASS()
class CHELIOS_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* ItemQuantity;

};
