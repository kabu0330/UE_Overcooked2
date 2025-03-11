// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

void AWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), TEXT("WorldRoad"), RoadActors);
}

void AWorldGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	int a = 0;

	if (IdxOpenRoad >= 0 && IdxOpenRoad < RoadActors.Num())
	{
		//RoadActors[IdxOpenRoad]->SetActorRelativeScale3D();
	}
}

void AWorldGameMode::OpenRoad1_1(int _Idx)
{
	IdxOpenRoad = _Idx;
}