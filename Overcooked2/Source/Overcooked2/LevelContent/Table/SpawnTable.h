// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include <Global/OC2Enum.h>
#include "SpawnTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ASpawnTable : public ACookingTable
{
	GENERATED_BODY()
	
public:
	ASpawnTable();
	
	UFUNCTION(BlueprintCallable)
	void Init(FName Name);

	UFUNCTION(BlueprintCallable)
	class AIngredient* SpawnIngredient(AActor* ChefActor);

	UFUNCTION(BlueprintCallable)
	virtual void Interact(AActor* ChefActor) override;

protected:

private:
	FName IngredientName = FName();
};
