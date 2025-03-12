// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include "FireExtinguisher.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AFireExtinguisher : public ACooking
{
	GENERATED_BODY()
	
public:
	AFireExtinguisher();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};
