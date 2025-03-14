// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldManager.generated.h"

UENUM()
enum class EWorldState
{
	Idle = 0,
	ShowStage1_1,
	WaitTile1_1,
	ShowEnvs1_1,
	ShowSavedLoc,
};

UCLASS()
class OVERCOOKED2_API AWorldManager : public AActor
{
	GENERATED_BODY()

public:
	AWorldManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float _DeltaTime) override;

private:
	void ChangeState(EWorldState _State);
	void SetPlayersToSpectators();
	void ShowPlayer();

	void ChangeCameraToWorld();
	void ChangeCameraToPlayer();

	void OnIdle();
	void OnShowEnvs1_1();

	void RunShowStage1_1(float _DeltaTime);
	void RunShowSavedLoc(float _DeltaTime);
	void RunWaitTile1_1(float _DeltaTime);
	void RunShowEnvs1_1(float _DeltaTime);

	UPROPERTY()
	TArray<AActor*> ActorsChangingScale3D;

	UPROPERTY()
	TArray<AActor*> ActorsChangingVisibility;

	UPROPERTY()
	TArray<AActor*> ActorsChangingScaleY;

	UPROPERTY()
	class ATileGrid* Tile1_1 = nullptr;

	UPROPERTY()
	bool IsOpen1_1 = false;

	// Temp
	int IdxOpenRoad = -1;
	int IdxOpenBuilding = -1;
	float ElapsedEnv = 0.f;
	const float ENV_SEC = 1.f;

	EWorldState StageState = EWorldState::Idle;

	TObjectPtr<AActor> WorldCamera = nullptr;
};
