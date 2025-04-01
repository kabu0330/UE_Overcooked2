// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/State/OC2GameState.h"

#include "Global/OC2Const.h"
#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"

#include "CookingGameState.generated.h"

class ASpawnManager;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingGameState : public AOC2GameState
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreateNewOrder(FOrder Order);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CompleteOrder(FOrder Order);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BlinkOrderUI();

private:
	int FindOrderIndex(FOrder& Order);

private:
	TArray<FOrder> OrderList;
	
};
