// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Struct.h"
#include "Global/Data/IngredientDataTable.h"
#include "OrderDataTable.generated.h"


// 주문 데이터 행
USTRUCT(BlueprintType)
struct FOrderDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FOrderDataRow() {}
	~FOrderDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UTexture2D* OrderTexutre = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FCookableIngredient> RequireIngredients;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOrderDataTable : public UObject
{
	GENERATED_BODY()
	
};
