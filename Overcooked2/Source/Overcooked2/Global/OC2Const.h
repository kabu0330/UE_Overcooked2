// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OC2Const.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Const : public UObject
{
	GENERATED_BODY()

public:
	static float OrderSpawnDelay;
	static float MaxOrderCount;
	static float OC2CharacterSizeY;

	static float BGSoundVolume;
	static float AmbienceVolume;
	static float EffectVolume;
	static float UIVolume;

	static int ScoreValue;
	static int TipValue;

	static FString ChefEagleHeadName;
	static FString ChefMouseHeadName;
	static FString ChefPandaHeadName;
	static FString ChefPigHeadName;
	static FString ChefOwlHeadName;

	static FString ChefTextureName;

	static FVector TitleCharacterSpawnLocation;
	static FRotator TitleCharacterSpawnRotation;

	static FVector PlateSubmitLocation;
	
};
