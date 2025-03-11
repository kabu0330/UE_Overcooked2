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
	APrepTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION(BlueprintCallable)
	virtual void SetAndSpawnCooking(AActor* ChefActor, ACooking* CookingObj);*/

	UFUNCTION(BlueprintCallable)
	void SetAndSpawnCooking(AActor* ChefActor, EIngredientType IngredientType);

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void PlaceItem(ACooking* Item) override;

private:

	ACooking* PPlacedItem = nullptr;

};
