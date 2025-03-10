// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CookingDevGameState.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACookingDevGameState()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	
};
