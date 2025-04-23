// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceWorldSoundWaveTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceWorldSoundWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceWorldSoundWaveDataRow() {}
	~FResourceWorldSoundWaveDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	USoundBase* SoundBase = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceWorldSoundWaveTable : public UObject
{
	GENERATED_BODY()
	
};
