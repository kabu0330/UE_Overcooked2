// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Global/OC2Const.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Struct.h"

#include "SpawnManager.generated.h"

class ACookingTable;
class APlateSpawner;
class APlate;
class APot;

UCLASS()
class OVERCOOKED2_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

	UFUNCTION(BlueprintCallable)
	static ASpawnManager* Get(UWorld* World);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|SpawnActor")
	TSubclassOf<APlate> PlateClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|SpawnActor")
	TSubclassOf<APot> PotClass = nullptr;

private:
	APlate* Plate = nullptr;
	float CheckTime = 0.0f;
	APlateSpawner* PlateSpawner = nullptr;

};
