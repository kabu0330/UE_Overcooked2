// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Map/OC2Map.h"

AOC2Map::AOC2Map()
{
	FVector Scale(100.0f, 100.0f, 100.0f);
	SetActorRelativeScale3D(Scale);

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//StaticMeshComponent->SetCollisionResponseToChannels
	StaticMeshComponent->ComponentTags.Add(TEXT("Floor"));
}

void AOC2Map::BeginPlay()
{
	AOC2Actor::BeginPlay();
}

void AOC2Map::Tick(float DeltaTime)
{
	AOC2Actor::Tick(DeltaTime);
}
