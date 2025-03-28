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

	static FString ChefEagleHeadName;
	static FString ChefMouseHeadName;
	static FString ChefPandaHeadName;
	static FString ChefPigHeadName;

	static FVector TitleCharacterSpawnLocation;
	static FRotator TitleCharacterSpawnRotation;
	
};
