// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/State/OC2GameState.h"
#include "WorldGameState.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AWorldGameState : public AOC2GameState
{
	GENERATED_BODY()

public:
	AWorldGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayZoomInAnmationUI();
	
};
