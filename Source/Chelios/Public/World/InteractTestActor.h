// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InteractTestActor.generated.h"

UCLASS()
class CHELIOS_API AInteractTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
//======================================================================
// FUNCTIONS
//======================================================================

	AInteractTestActor();
	virtual void Tick(float DeltaTime) override;

protected:
//======================================================================
// FUNCTIONS
//======================================================================

	virtual void BeginPlay() override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual	void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ACheliosCharacter* PlayerCharacter) override;

//======================================================================
// PROPERTIES & VARIABLES
//======================================================================

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;

};
