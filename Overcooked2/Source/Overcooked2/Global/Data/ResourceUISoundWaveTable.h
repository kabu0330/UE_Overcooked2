// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceUISoundWaveTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceUISoundWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceUISoundWaveDataRow() {}
	~FResourceUISoundWaveDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	USoundBase* SoundBase = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceUISoundWaveTable : public UObject
{
	GENERATED_BODY()
	
};
