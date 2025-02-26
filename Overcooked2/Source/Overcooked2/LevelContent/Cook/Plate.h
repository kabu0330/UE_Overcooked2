// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>
//#include <Global/Interface/InterActable.h>
#include "Plate.generated.h"

// 접시 ~ 접시에 올라간 조리된 요리 ~ 요리들의 조합 ~ 완성된 요리
UCLASS()
class OVERCOOKED2_API APlate : public ACooking/*, public UInterActable*/
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlate();

	UFUNCTION(BlueprintCallable)
	void CleanPlate()
	{
		bCanPlaceIngredient = true;
	}

	bool Add(class AIngredient* Ingredient)
	{
		// 예외처리 로직 필요
		//if (nullptr == Ingredient || Ingredient.GetIngredientState() != EIngredientState::COOKED)
		//{
		//	return false;
		//}

		// TArray에 Add
		return true;
	}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// TArray가 있어야 할 듯?
	// FCookableIngredient CookedIngredient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	bool bCanPlaceIngredient = true;
};
