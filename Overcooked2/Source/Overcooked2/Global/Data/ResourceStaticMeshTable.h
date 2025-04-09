// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceStaticMeshTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceStaticMeshDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceStaticMeshDataRow() {}
	~FResourceStaticMeshDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* StaticMesh = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceStaticMeshTable : public UObject
{
	GENERATED_BODY()
	
};
