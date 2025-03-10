// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/SpawnManager.h"
#include <LevelContent/Cook/Dev/CookingDevGameMode.h>
#include <Net/UnrealNetwork.h>

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

void ASpawnManager::SetIngredientType(UWorld* World, EIngredientType Type)
{
	IngredientType = Type;
	RequestSpawnIngredient(World);
}

void ASpawnManager::RequestSpawnIngredient_Implementation(UWorld* World)
{
	ACookingDevGameMode* GameMode = Cast<ACookingDevGameMode>(World->GetAuthGameMode());
	if (nullptr != GameMode)
	{
		GameMode->SpawnIngredient(IngredientType);
	}
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnManager, IngredientType);
}

