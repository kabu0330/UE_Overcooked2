// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameMode/OC2GameMode.h"

#include "Global/Component/SpawnManageComponent.h"
#include "LevelContent/Cook/Ingredient.h"
#include "LevelContent/Cook/Plate.h"

AOC2GameMode::AOC2GameMode()
{
	SpawnManager = CreateDefaultSubobject<USpawnManageComponent>(TEXT("SpawnManager"));
}

void AOC2GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AOC2GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOC2GameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);
}

AIngredient* AOC2GameMode::SpawnIngredientActor(EIngredientType IngredientType)
{
	return SpawnManager->SpawnIngredientActor(IngredientType);
}

APlate* AOC2GameMode::SpawnPlateActor()
{
	return SpawnManager->SpawnPlateActor(PlateToSpawn);
}
