// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
#include "LevelContent/WorldMap/TileGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

void AWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), TEXT("WorldRoad"), RoadActors);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATileGrid::StaticClass(), TEXT("TileGrid"), Actors);

	if (Actors.Num() > 0)
	{
		Tile1_1 = Cast<ATileGrid>(Actors[0]);
	}
}

void AWorldGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	int a = 0;

	if (Tile1_1->IsEndTransition())
	{
		OpenRoad1_1(0);		// Temp
	}

	if (IdxOpenRoad >= 0 && IdxOpenRoad < RoadActors.Num())
	{
		float X = RoadActors[IdxOpenRoad]->GetActorScale().X;
		if (FMath::IsNearlyEqual(X, 1.0, 1e-2))
		{
			IdxOpenRoad = -1;
			return;
		}

		RoadActors[IdxOpenRoad]->SetActorRelativeScale3D(FVector(X + 0.05f, 1.f, 1.f));
	}
}

void AWorldGameMode::OpenRoad1_1(int _Idx)
{
	if (IdxOpenRoad < 0)
	{
		IdxOpenRoad = _Idx;
	}
}