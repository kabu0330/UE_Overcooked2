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

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayZoomInAnmationUI();

	UFUNCTION(Server, Reliable)
	void Server_AddUserCount();

public:
	UFUNCTION()
	void CheckClinetLoadingComplete();

private:
	UPROPERTY()
	TArray<int> CompleteArray;

	UPROPERTY(Replicated)
	int CurUserCount = 0;
	
};
