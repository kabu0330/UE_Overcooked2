// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include <Global/GameMode/OC2GameMode.h>
#include <Global/OC2Enum.h>
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Plate.h>
#include <LevelContent/Cook/Dev/CookingDevPlayerState.h>
#include "CookingDevGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevGameMode : public AOC2GameMode
{
	GENERATED_BODY()
	
public:
	ACookingDevGameMode();


	UFUNCTION(BlueprintCallable)
	class UCookingDevUserWidget* GetWidget()
	{
		return Widget;
	}

	void SetWidget(class UCookingDevUserWidget* UserWidget)
	{
		Widget = UserWidget;
	}

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
		Ingredients.Empty();
		Plates.Empty();
	}

	void ChangeState(EIngredientState State);

	void PlaceOnthePlate();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	

private:
	class UCookingDevUserWidget* Widget = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<AIngredient*> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<APlate*> Plates;

	TArray<APlate*> Dish;
};
