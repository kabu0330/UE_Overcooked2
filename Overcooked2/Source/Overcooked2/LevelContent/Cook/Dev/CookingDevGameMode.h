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
#include <LevelContent/Cook/Pot.h>
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
		Ingredient->AddActorWorldOffset(FVector(0, 0, 20));
		CookingObjectManager->GetIngredients().Add(Ingredient);
	}

	void AddPlate(APlate* Plate)
	{
		Plate->SetActorLocation(FVector(300, -100, 10));
		CookingObjectManager->GetPlates().Add(Plate);
	}

	void PlaceOnThePot();

	void Wash();

	void Reset()
	{
		CookingObjectManager->Reset();
	}

	void CleanPlate();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	

private:
	class UCookingDevUserWidget* Widget = nullptr;

	UCookingObjectManager* CookingObjectManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APot> SubclassPot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	APot* Pot = nullptr;

};
