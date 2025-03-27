// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldFlag.h"
#include "LevelContent/WorldMap/WorldMapDialogue.h"
#include "Components/BillboardComponent.h"

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

	SetActorHiddenInGame(true);

	if (StageCount > 0)
	{
		UWorld* Level = GetWorld();
		if (Level)
		{
			WorldMapDialogue = Level->SpawnActor<AWorldMapDialogue>();
			WorldMapDialogue->SetStage(StageCount);
			WorldMapDialogue->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			WorldMapDialogue->SetActorRelativeScale3D(FVector(2.f, 2.f, 1.f));
			WorldMapDialogue->SetActorHiddenInGame(true);
		}
	}
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
		else if (_State == EStageExplain::Idle)
		{
			OnIdle();
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

void AWorldFlag::OnIdle()
{
	if (WorldMapDialogue->IsHidden() == false)
	{
		WorldMapDialogue->SetActorHiddenInGame(true);
	}
}

void AWorldFlag::OnShowExplain()
{
	if (WorldMapDialogue->IsHidden())
	{
		WorldMapDialogue->SetActorHiddenInGame(false);
	}
}
