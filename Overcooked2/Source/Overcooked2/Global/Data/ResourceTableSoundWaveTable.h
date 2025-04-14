// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceTableSoundWaveTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceTableSoundWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceTableSoundWaveDataRow() {}
	~FResourceTableSoundWaveDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	USoundBase* SoundBase = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceTableSoundWaveTable : public UObject
{
	GENERATED_BODY()
	
};
