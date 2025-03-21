// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "ServingTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AServingTable : public ACookingTable
{
	GENERATED_BODY()

public:
	AServingTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
