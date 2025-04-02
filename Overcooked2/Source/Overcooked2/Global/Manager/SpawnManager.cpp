// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Manager/SpawnManager.h"
#include "LevelContent/Table/CookingTable.h"
#include "EngineUtils.h"

#include "Global/GameMode/CookingGameMode.h"
#include "Global/State/GameState/CookingGameState.h"

#include "LevelContent/Cook/Plate.h"
#include "LevelContent/Cook/Pot.h"
#include "LevelContent/Table/NonTable/PlateSpawner.h"

ASpawnManager::ASpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

ASpawnManager* ASpawnManager::Get(UWorld* World)
{
	if (ACookingGameMode* CookingGameMode = Cast<ACookingGameMode>(World->GetAuthGameMode()))
	{
		return CookingGameMode->SpawnManagerActor;
	}

	return nullptr;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	// TActorIterator를 사용하여 월드 내 모든 APrepTable 액터를 순회
	for (TActorIterator<ACookingTable> It(GetWorld()); It; ++It)
	{
		ACookingTable* PrepTableActor = *It;
		if (PrepTableActor->Tags.Contains("Plate"))
		{
			APlate* SpawnedPlate = GetWorld()->SpawnActor<APlate>(PlateClass);
			PrepTableActor->PlaceItem(SpawnedPlate);
		}
		else if (PrepTableActor->Tags.Contains("Pot"))
		{
			APot* Pot = GetWorld()->SpawnActor<APot>(PotClass);
			PrepTableActor->PlaceItem(Pot);
		}
		else if (PrepTableActor->Tags.Contains("PlateSpawner"))
		{
			PlateSpawner = Cast<APlateSpawner>(PrepTableActor);
		}
	}
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

