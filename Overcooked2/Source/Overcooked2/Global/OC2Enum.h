// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OC2Enum.generated.h"


UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	EIT_NONE			UMETA(DisplayName = "None"),
	EIT_FISH			UMETA(DisplayName = "생선"),
	//EIT_CABBAGE			UMETA(DisplayName = "양배추"),
	EIT_PRAWN			UMETA(DisplayName = "새우"),
	//EIT_TOMATO			UMETA(DisplayName = "토마토"),
	EIT_SEAWEED			UMETA(DisplayName = "김"),
	EIT_RICE			UMETA(DisplayName = "쌀"),
	EIT_CUCUMBER		UMETA(DisplayName = "오이"),
};

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	EIS_NONE      		UMETA(DisplayName = "None"),
	EIS_CHOPPABLE		UMETA(DisplayName = "손질 전"),
	EIS_CHOPPED   		UMETA(DisplayName = "손질 후"),
	EIS_GRILLABLE    	UMETA(DisplayName = "굽기 전"),
	EIS_GRILLED      	UMETA(DisplayName = "구운 후"),
	EIS_FRYABLE     	UMETA(DisplayName = "튀기기 전"),
	EIS_FRYABED     	UMETA(DisplayName = "튀기기 후"),
	EIS_BOILABLE     	UMETA(DisplayName = "끓이기 전"),
	EIS_BOILED      	UMETA(DisplayName = "끓이기 후"),
	EIS_FINISHED     	UMETA(DisplayName = "완제품"),
	EIS_OVERCOOKED      UMETA(DisplayName = "태움"),
	EIS_MAX				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class ECookingType : uint8
{
	ECT_NONE				UMETA(DisplayName = "None"),
	ECT_INGREDIENT			UMETA(DisplayName = "재료"),
	ECT_PLATE				UMETA(DisplayName = "요리"),
	ECT_POT					UMETA(DisplayName = "냄비"),
	ECT_FIREEXTINGUISHER	UMETA(DisplayName = "소화기"),
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Enum : public UObject
{
	GENERATED_BODY()
	
};
