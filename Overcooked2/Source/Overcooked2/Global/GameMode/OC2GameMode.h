// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Const.h"

#include "OC2GameMode.generated.h"

class AIngredient;
class APlate;
class AOC2Actor;
class APot;

class USpawnManageComponent;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AOC2GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AOC2GameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayerController) override;

public:
	AIngredient* SpawnIngredientActor(EIngredientType Type);
	APlate* SpawnPlateActor(EPlateState PlateState = EPlateState::EMPTY);
	APot* SpawnPotActor(EPotState PotState);

	void DestroyOC2Actor(AOC2Actor* OC2Actor);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	USpawnManageComponent* SpawnManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AIngredient> IngredientToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlate> PlateToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APot> PotToSpawn;
};
