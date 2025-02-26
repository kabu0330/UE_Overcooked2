// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "PrepTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APrepTable : public ACookingTable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Interact(AActor* ChefActor) override;

};
