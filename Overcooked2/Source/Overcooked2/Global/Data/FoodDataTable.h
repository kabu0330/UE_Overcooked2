// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FoodDataTable.generated.h"

UENUM(BlueprintType)
enum class EFoodDataType : uint8
{
	EFD_None	UMETA(DisplayName = "None"),
	EFD_Fish    UMETA(DisplayName = "Fish"),
	EFD_Cabbage UMETA(DisplayName = "Cabbage"),
	EFD_Tomato  UMETA(DisplayName = "Tomato"),
};

USTRUCT(BlueprintType)
struct FFoodDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FFoodDataRow() {}
	~FFoodDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EFoodDataType FoodDataType = EFoodDataType::EFD_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	bool bCooked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	bool bBoilable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	bool bGrillable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	bool bCuttable = false;
};
/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UFoodDataTable : public UObject
{
	GENERATED_BODY()
};
