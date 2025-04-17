// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Global/GameMode/OC2GameMode.h"
#include "WorldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AWorldGameMode : public AOC2GameMode
{
	GENERATED_BODY()

public:
	AWorldGameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitGameState() override;
	virtual void PostLogin(APlayerController* NewPlayerController) override;

public:
	int GetUserCount()
	{
		return CurUserCount;
	}

private:
	TObjectPtr<class AWorldManager> WorldManager = nullptr;
	TArray<APlayerController*> PlayerControllers;

	UPROPERTY()
	int CurUserCount = 0;
};
