// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include <Global/GameMode/OC2GameMode.h>
#include <Global/OC2Enum.h>
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Plate.h>
#include <LevelContent/Cook/Dev/CookingDevPlayerState.h>
#include <LevelContent/Cook/Dev/CookingObjectManager.h>
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

	void ChangeState(EIngredientState State);

	void PlaceOnthePlate();

	void AddIngredient(AIngredient* Ingredient)
	{
		CookingObjectManager->GetIngredients().Add(Ingredient);
	}

	void AddPlate(APlate* Plate)
	{
		CookingObjectManager->GetPlates().Add(Plate);
	}

	void Wash();

	void Reset()
	{
		CookingObjectManager->Reset();
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	

private:
	class UCookingDevUserWidget* Widget = nullptr;

	UCookingObjectManager* CookingObjectManager = nullptr;

};
