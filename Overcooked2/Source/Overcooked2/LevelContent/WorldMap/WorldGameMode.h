// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "WorldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AWorldGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AWorldGameMode();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	TObjectPtr<class AWorldManager> WorldManager = nullptr;
};
