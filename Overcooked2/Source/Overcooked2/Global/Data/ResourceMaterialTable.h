// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceMaterialTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceMaterialDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceMaterialDataRow() {}
	~FResourceMaterialDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UMaterial* Material = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceMaterialTable : public UObject
{
	GENERATED_BODY()
	
};
