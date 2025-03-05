// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global/OC2Struct.h"
#include "RecipeDataTable.generated.h"

// 레시피 데이터 행
USTRUCT(BlueprintType)
struct FRecipeDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FRecipeDataRow() {}
	~FRecipeDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UTexture2D* OrderTexutre = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FCookableIngredient> RequireIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* FoodMesh = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API URecipeDataTable : public UObject
{
	GENERATED_BODY()
	
};
