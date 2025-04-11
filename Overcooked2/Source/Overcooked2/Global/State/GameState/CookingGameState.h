// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/State/OC2GameState.h"

#include "Global/OC2Const.h"
#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"

#include "CookingGameState.generated.h"

class ASpawnManager;
class APlate;
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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void WaitingToStart(float DeltaTime);
	void InProgress(float DeltaTime);
	void WaitingPostMatch(float DeltaTime);


public:
	void ChangeState(ECookingStageState ChangeState);
	void EntryWaitingToStart();
	void EntryInProgress();
	void EntryWaitingPostMatch();

	void AddPlate(APlate* Plate);
	APlate* GetPlate(int Index);

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreateNewOrder(FOrder Order);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CompleteOrder(int OrderIndex, int InScore);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BlinkOrderUI();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddScore(int InScore);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartGame();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SettingTimer(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetFeverUI(int InFeverCount);

	UFUNCTION(Server, Reliable)
	void Server_SubmitPlate(ACooking* Plate);

	UFUNCTION(Server, Reliable)
	void Server_MovePlate(ACooking* Cooking);

	virtual void OnRep_MatchState() override;

private:
	int FindOrderIndex(FOrder& Order);

private:
	TArray<FOrder> OrderList;

	UPROPERTY(Replicated)
	ECookingStageState CurStatgeState = ECookingStageState::ESS_NONE;

	float CurTime = 0.0f;
	int Score = 0;
	TArray<int> OrderNumberArray;
	TArray<FOrder> OrderArray;
	TArray<APlate*> PlateArray;
	int CurOrderIndex = 0;
	int FeverCount = 0;
};
