// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Plate.h>
#include "CookingObjectManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API UCookingObjectManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCookingObjectManager();

	void AddIngredient(AIngredient* Ingredient)
	{
		Ingredients.Add(Ingredient);
	}

	void AddPlate(APlate* Plate)
	{
		Plates.Add(Plate);
	}

	void Reset()
	{
		for (AIngredient* Ingredient : Ingredients)
		{
			Ingredient->Destroy();
		}
		for (APlate* Plate : Plates)
		{
			Plate->Destroy();
		}
		for (APlate* Dish : Dishes)
		{
			Dish->Destroy();
		}

		Ingredients.Empty();
		Plates.Empty();
		Dishes.Empty();
	}

	TArray<AIngredient*>& GetIngredients()
	{
		return Ingredients;
	}

	TArray<APlate*>& GetPlates()
	{
		return Plates;
	}

	// Debug & Test
	TArray<APlate*>& GetDishes()
	{
		return Dishes;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<AIngredient*> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<APlate*> Plates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<APlate*> Dishes;
};
