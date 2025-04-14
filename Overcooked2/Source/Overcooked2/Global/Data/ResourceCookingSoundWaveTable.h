// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceCookingSoundWaveTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceCookingSoundWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceCookingSoundWaveDataRow() {}
	~FResourceCookingSoundWaveDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	USoundBase* SoundBase = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceCookingSoundWaveTable : public UObject
{
	GENERATED_BODY()
	
};
