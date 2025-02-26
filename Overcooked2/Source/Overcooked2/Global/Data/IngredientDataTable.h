// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global/OC2Enum.h"
#include "IngredientDataTable.generated.h"

USTRUCT(BlueprintType)
struct FIngredientCookDataRow 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientState PrevIngredientState = EIngredientState::EIS_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientState IngredientState = EIngredientState::EIS_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* CookMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FIngredientDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FIngredientDataRow() {}
	~FIngredientDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* BaseMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FIngredientCookDataRow> Rows;
};

USTRUCT(BlueprintType)
struct FCookableIngredient 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientState IngredientState = EIngredientState::EIS_NONE;

};

USTRUCT(BlueprintType)
struct FRecipeDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FCookableIngredient> CookableIngredients;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UIngredientDataTable : public UObject
{
	GENERATED_BODY()
	
};
