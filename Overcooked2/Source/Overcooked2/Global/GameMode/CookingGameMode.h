// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Const.h"

#include "CookingGameMode.generated.h"

class UOrderManageComponent;
class USpawnManageComponent;
class AIngredient;
class APlate;

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
class OVERCOOKED2_API ACookingGameMode : public AGameMode
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
	AIngredient* SpawnIngredientActor(EIngredientType Type);
	APlate* SpawnPlateActor();

	TArray<APlayerController*> PlayerControllers;

private:
	void PrintDebugMessage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Order", meta = (AllowPrivateAccess = "true"))
	UOrderManageComponent* OrderManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	USpawnManageComponent* SpawnManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AIngredient> IngredientToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlate> PlateToSpawn;
	
	ECookingGameModeState CurState = ECookingGameModeState::ECS_None;

	float CheckTime = 0.0f;
};
