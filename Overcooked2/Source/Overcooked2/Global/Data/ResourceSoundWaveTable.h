// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceSoundWaveTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceSoundWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceSoundWaveDataRow() {}
	~FResourceSoundWaveDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	USoundBase* SoundBase = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceSoundWaveTable : public UObject
{
	GENERATED_BODY()
	
};
