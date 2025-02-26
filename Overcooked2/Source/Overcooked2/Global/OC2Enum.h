// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OC2Enum.generated.h"

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	EIS_NONE      		UMETA(DisplayName = "None"),
	EIS_CHOPPABLE		UMETA(DisplayName = "채썰기"),
	EIS_GRILLABLE    	UMETA(DisplayName = "굽기"),
	EIS_FRYABLE     	UMETA(DisplayName = "튀기기"),
	EIS_BOILABLE     	UMETA(DisplayName = "끓이기"),
	EIS_OVERCOOKED      UMETA(DisplayName = "태움"),
	EIS_MAX				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	EIT_NONE			UMETA(DisplayName = "None"),
	EIT_FISH			UMETA(DisplayName = "생성"),
	EIT_CABBAGE			UMETA(DisplayName = "양배추"),
	EIT_TOMATO			UMETA(DisplayName = "토마토"),
};

UENUM(BlueprintType)
enum class ECookingType : uint8
{
	ECT_NONE			UMETA(DisplayName = "None"),
	ECT_INGREDIENT		UMETA(DisplayName = "Ingredient"),
	ECT_PLATE			UMETA(DisplayName = "Plate"),
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Enum : public UObject
{
	GENERATED_BODY()
	
};
