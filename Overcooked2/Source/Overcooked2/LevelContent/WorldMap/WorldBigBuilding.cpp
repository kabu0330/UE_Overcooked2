// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldBigBuilding.h"

// Sets default values
AWorldBigBuilding::AWorldBigBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldBigBuilding::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
	
}

// Called every frame
void AWorldBigBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

