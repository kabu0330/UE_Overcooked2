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

	void OpenRoad1_1(int _Idx);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UPROPERTY()
	TArray<AActor*> ShowingActors1_1;

	UPROPERTY()
	TArray<AActor*> RoadActors;

	UPROPERTY()
	bool IsOpen1_1 = false;

	int IdxOpenRoad = -1;
};
