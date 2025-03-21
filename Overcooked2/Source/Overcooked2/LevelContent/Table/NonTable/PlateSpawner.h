// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "PlateSpawner.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APlateSpawner : public ACookingTable
{
	GENERATED_BODY()

public:
	APlateSpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
