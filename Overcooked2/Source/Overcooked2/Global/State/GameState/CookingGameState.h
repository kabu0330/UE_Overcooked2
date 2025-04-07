// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/State/OC2GameState.h"

#include "Global/OC2Const.h"
#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"

#include "CookingGameState.generated.h"

class ASpawnManager;
class ACooking;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingGameState : public AOC2GameState
{
	GENERATED_BODY()

public:
	ACookingGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreateNewOrder(FOrder Order);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CompleteOrder(int OrderIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BlinkOrderUI();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddScore(int Score);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartGame();

	UFUNCTION(Server, Reliable)
	void Server_SubmitPlate(ACooking* Plate);

private:
	int FindOrderIndex(FOrder& Order);

private:
	TArray<FOrder> OrderList;
	
};
