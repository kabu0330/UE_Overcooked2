// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceCharacterSoundWaveTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceCharacterSoundWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceCharacterSoundWaveDataRow() {}
	~FResourceCharacterSoundWaveDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	USoundBase* SoundBase = nullptr;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceCharacterSoundWaveTable : public UObject
{
	GENERATED_BODY()
	
};
