// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldMapActor.h"

// Sets default values
AWorldMapActor::AWorldMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldMapActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

}

// Called every frame
void AWorldMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

