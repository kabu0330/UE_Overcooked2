// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldMapData.generated.h"

UENUM()
enum class EStageState : uint8
{
	None = 0,
	ShowStage1_1,
	HideStage1_1,
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UWorldMapData : public UObject
{
	GENERATED_BODY()
	
};
