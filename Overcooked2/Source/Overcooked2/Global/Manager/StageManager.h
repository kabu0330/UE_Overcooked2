// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Global/OC2Const.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Struct.h"

#include "StageManager.generated.h"

class ACookingGameState;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AStageManager : public AActor
{
	GENERATED_BODY()
	
public:
	AStageManager();

	UFUNCTION(BlueprintCallable)
	static AStageManager* Get(UWorld* WorldContext);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Global|CookingStage")
	void AddScore(int Value);

	UFUNCTION(BlueprintCallable, Category = "Global|CookingStage")
	int CompleteOrder(FOrder Order, int Score);

private:
	int FindOrderIndex(FOrder& Order);

public:
	ACookingGameState* CookingGameState = nullptr;
	int Score = 0;
	float CurTime = 0.0f;
	TArray<int> OrderNumberArray;
	TArray<FOrder> OrderArray;
	int CurOrderIndex = 0;
	bool bProgress = false;
};
