// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "BurnerTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ABurnerTable : public ACookingTable
{
	GENERATED_BODY()

public:
	ABurnerTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void PlaceItem(ACooking* Item) override;

	/*UFUNCTION(BlueprintCallable)
	void BoilThePot(AActor* ChefActor);*/

private:
	//ACooking* BPlacedItem = nullptr;
};
