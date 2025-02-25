// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OC2Enum.generated.h"

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	EFS_NONE      		UMETA(DisplayName = "None"),
	EFS_COOKED			UMETA(DisplayName = "Cooked"),
	EFS_CHOPPABLE		UMETA(DisplayName = "Choppabable"),
	EFS_GRILLABLE    	UMETA(DisplayName = "Grillable"),
	EFS_FRYABLE     	UMETA(DisplayName = "Fryable"),
	EFS_BOILABLE     	UMETA(DisplayName = "Boilable"),
	EFS_OVERCOOKED      UMETA(DisplayName = "Overcooked"),
	EFS_MAX				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	EFD_NONE	UMETA(DisplayName = "None"),
	EFD_FISH    UMETA(DisplayName = "Fish"),
	EFD_CABBAGE UMETA(DisplayName = "Cabbage"),
	EFD_TOMATO  UMETA(DisplayName = "Tomato"),
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Enum : public UObject
{
	GENERATED_BODY()
	
};
