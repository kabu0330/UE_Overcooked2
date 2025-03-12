// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Global/GameMode/OC2GameMode.h"

#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Const.h"

#include "CookingGameMode.generated.h"

class UOrderManageComponent;
class USpawnManageComponent;
class AIngredient;
class APlate;
class AOC2Actor;

UENUM(BlueprintType)
enum class ECookingGameModeState : uint8
{
	ECS_None	UMETA(DisplayName = "None"),
	ECS_Stay    UMETA(DisplayName = "Stay"),
	ECS_Stage   UMETA(DisplayName = "Stage"),
	ECS_Score   UMETA(DisplayName = "Score"),
};
/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingGameMode : public AOC2GameMode
{
	GENERATED_BODY()

public:
	ACookingGameMode();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayerController) override;
	
public:
	void EntryStay();
	void Stay(float DeltaTime);

	void EntryStage();
	void Stage(float DeltaTime);

	void EntryScore();
	void Score(float DeltaTime);

	void ChangeState(ECookingGameModeState State);

public:
	TArray<APlayerController*> PlayerControllers;

private:
	void PrintDebugMessage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Order", meta = (AllowPrivateAccess = "true"))
	UOrderManageComponent* OrderManager = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOC2Actor> OC2ActorToSpawn;
	
	ECookingGameModeState CurState = ECookingGameModeState::ECS_None;

	float CheckTime = 0.0f;
};
