// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
#include "LevelContent/WorldMap/TileGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

// TODO: literal -> variable
void AWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), TEXT("WorldRoad"), RoadActors);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Building1"), ActorsChangingScale3D);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Stone1"), ActorsChangingVisibility);	// TODO: merge

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATileGrid::StaticClass(), TEXT("TileGrid"), Actors);

	if (Actors.Num() > 0)
	{
		Tile1_1 = Cast<ATileGrid>(Actors[0]);
	}

	for (int i = 0; i < ActorsChangingVisibility.Num(); ++i)
	{
		ActorsChangingVisibility[i]->SetActorHiddenInGame(true);
	}
}

void AWorldGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	int a = 0;

	if (Tile1_1->IsEndTransition())
	{
		// Temp
		OpenRoad1_1(0);
		ShowBuilding1_1();
		ShowStone1();	// TODO: check overlap
	}

	// TODO: Move to function
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
	
	// TODO: Move to function
	if (IsBuildingShow1_1)
	{
		FVector Scale = ActorsChangingScale3D[0]->GetActorScale();	// Temp
		if (FMath::IsNearlyEqual(Scale.X, 1.0, 1e-2))
		{
			IsBuildingShow1_1 = false;
			return;
		}

		for (int i = 0; i < ActorsChangingScale3D.Num(); ++i)
		{
			ActorsChangingScale3D[i]->SetActorRelativeScale3D(Scale + 0.05f);
		}
	}
}

void AWorldGameMode::ShowStone1()
{
	for (int i = 0; i < ActorsChangingVisibility.Num(); ++i)
	{
		ActorsChangingVisibility[i]->SetActorHiddenInGame(false);
	}
}

void AWorldGameMode::ShowBuilding1_1()
{
	IsBuildingShow1_1 = true;
}

void AWorldGameMode::OpenRoad1_1(int _Idx)
{
	if (IdxOpenRoad < 0)
	{
		IdxOpenRoad = _Idx;
	}
}