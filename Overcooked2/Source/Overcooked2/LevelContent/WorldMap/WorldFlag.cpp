// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldFlag.h"

AWorldFlag::AWorldFlag()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxColiision");
	BoxCollision->SetupAttachment(RootComponent);

	FlagMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("FlagMesh");
	FlagMeshComponent->SetupAttachment(RootComponent);

	FloorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("FloorMesh");
	FloorMeshComponent->SetupAttachment(RootComponent);
}

void AWorldFlag::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldFlag::ChangeState(EStageExplain _State)
{
	if (State != _State)
	{
		if (_State == EStageExplain::ShowExplain)
		{
			OnShowExplain();
		}
	}

	State = _State;
}

void AWorldFlag::OnOverlapStarted(AActor* _OtherActor)
{
	if (State != EStageExplain::Idle)
	{
		return;
	}

	ChangeState(EStageExplain::ShowExplain);
}

void AWorldFlag::OnOverlapFinished(AActor* _OtherActor)
{
	if (State == EStageExplain::Idle)
	{
		return;
	}

	ChangeState(EStageExplain::Idle);
}

void AWorldFlag::OnShowExplain()
{
	// Temp
	if (StageCount == 1)
	{

	}
}
