// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OVERCOOKED2_API IInteractableObject
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* ChefActor) = 0;
	
};
