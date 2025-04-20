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
class ADirectionalLight;
class AOC2Map;
class AOC2CameraActor;

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

public:
	void AddPlate(APlate* Plate);
	APlate* GetPlate(int Index);

	int GetOrderScore();
	int GetFeverScore();
	int GetFailScore();
	int GetTotalScore();
	int GetPlateArrayNum();
	void OnPlayerCookingWidgetReady();

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

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayGameMapSound();

	UFUNCTION(Server, Reliable)
	void Server_SubmitPlate(ACooking* Plate);

	UFUNCTION(Server, Reliable)
	void Server_MovePlate(ACooking* Cooking);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowTimesUpUI();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCharacterActive(bool bActive);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCharacterHead();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowScorePanemUI();

	UFUNCTION(NetMulticast, Reliable)
	void Muticast_EndGame();

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
	TArray<int> ReadyPlayer;
	int CurOrderIndex = 0;
	int FeverCount = 0;
	ADirectionalLight* DirectionalLight = nullptr;
	AOC2Map* OC2Map = nullptr;
	AOC2CameraActor* CameraActor = nullptr;

	UPROPERTY(Replicated)
	int OrderScore = 0;

	UPROPERTY(Replicated)
	int FeverScore = 0;

	UPROPERTY(Replicated)
	int FailScore = 0;

	UPROPERTY(Replicated)
	int TotalScore = 0;
};
