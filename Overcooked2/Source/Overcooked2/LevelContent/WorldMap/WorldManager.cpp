// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldManager.h"
#include "LevelContent/WorldMap/TileGrid.h"
#include "LevelContent/WorldMap/WorldPlayer.h"
#include "LevelContent/WorldMap/WorldPlayerController.h"
#include "Global/OC2GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Camera/CameraComponent.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}

// TODO: literal -> variable
void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), TEXT("WorldRoad"), ActorsChangingScaleY);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Building1"), ActorsChangingScale3D);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Stone1"), ActorsChangingVisibility);	// TODO: merge

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATileGrid::StaticClass(), TEXT("TileGrid"), Actors);
	if (Actors.Num() > 0)
	{
		Tile1_1 = Cast<ATileGrid>(Actors[0]);
	}

	Actors.Empty();
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("WorldCamera"), Actors);
	if (Actors.Num() > 0)
	{
		WorldCamera = Actors[0];

		// TODO: set original loc
		//WorldCamera->SetActorLocation();
	}

	if (WorldCamera == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("World Camera is null!"));
	}

	for (int i = 0; i < ActorsChangingVisibility.Num(); ++i)
	{
		ActorsChangingVisibility[i]->SetActorHiddenInGame(true);
	}

	ChangeCameraToWorld();
	//SetPlayersToSpectators();

	ChangeState(EWorldState::ShowStage1_1);
}

void AWorldManager::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (StageState == EWorldState::ShowStage1_1)
	{
		RunShowStage1_1(_DeltaTime);
	}
	else if (StageState == EWorldState::WaitTile1_1)
	{
		RunWaitTile1_1(_DeltaTime);
	}
	else if (StageState == EWorldState::ShowEnvs1_1)
	{
		RunShowEnvs1_1(_DeltaTime);
	}
	else if (StageState == EWorldState::ShowSavedLoc)
	{
		RunShowSavedLoc(_DeltaTime);
	}
}

void AWorldManager::ChangeState(EWorldState _State)
{
	if (StageState != _State)
	{
		if (_State == EWorldState::ShowStage1_1)
		{
			OnShowStage1_1();
		}
		else if (_State == EWorldState::ShowEnvs1_1)
		{
			OnShowEnvs1_1();
		}
		else if (_State == EWorldState::Idle)
		{
			OnIdle();
		}
	}

	StageState = _State;
}

void AWorldManager::OnIdle()
{
	ChangeCameraToPlayer();
	ToggleAllPlayersController(true);
}

void AWorldManager::OnShowStage1_1()
{
	ToggleAllPlayersController(false);
}

void AWorldManager::OnShowEnvs1_1()
{
	IdxOpenRoad = 0;
	IdxOpenBuilding = 0;

	// Show stones
	for (int i = 0; i < ActorsChangingVisibility.Num(); ++i)
	{
		ActorsChangingVisibility[i]->SetActorHiddenInGame(false);
	}
}

void AWorldManager::RunShowStage1_1(float _DeltaTime)
{
	FVector CurLoc = WorldCamera->GetActorLocation();
	FVector DestLoc(-320.f, 420.f, CurLoc.Z);	// Temp

	float Dist = FVector::Dist(DestLoc, CurLoc);
	if (Dist < 2.f)	// Temp
	{
		ChangeState(EWorldState::WaitTile1_1);
		WorldCamera->SetActorLocation(DestLoc);
		return;
	}

	FVector Dir = DestLoc - CurLoc;
	Dir.Normalize();
	Dir *= 2.f;
	FVector LocVec = CurLoc + Dir;
	WorldCamera->SetActorLocation(FVector(LocVec.X, LocVec.Y, CurLoc.Z));
}

void AWorldManager::RunShowSavedLoc(float _DeltaTime)
{
	FVector CurLoc = WorldCamera->GetActorLocation();
	FVector DestLoc(-100.f, 150.f, CurLoc.Z);	// Temp

	float Dist = FVector::Dist(DestLoc, CurLoc);
	if (Dist < 2.f)	// Temp
	{
		WorldCamera->SetActorLocation(DestLoc);
		ChangeState(EWorldState::Idle);
		return;
	}

	FVector Dir = DestLoc - CurLoc;
	Dir.Normalize();
	Dir *= 2.f;
	FVector LocVec = CurLoc + Dir;
	WorldCamera->SetActorLocation(FVector(LocVec.X, LocVec.Y, CurLoc.Z));
}

void AWorldManager::RunWaitTile1_1(float _DeltaTime)
{
	if (Tile1_1->IsStartedFlip() == false)
	{
		Tile1_1->Flip();
	}
	else if (Tile1_1->IsFliped())
	{
		ChangeState(EWorldState::ShowEnvs1_1);
	}
}

void AWorldManager::RunShowEnvs1_1(float _DeltaTime)
{
	bool IsAllEnd = true;
	if (IdxOpenRoad >= 0 && IdxOpenRoad < ActorsChangingScaleY.Num())
	{
		float X = ActorsChangingScaleY[IdxOpenRoad]->GetActorScale().X;
		if (FMath::IsNearlyEqual(X, 1.0, 1e-2))
		{
			IdxOpenRoad = -1;
		}
		else
		{
			IsAllEnd = false;
			ActorsChangingScaleY[IdxOpenRoad]->SetActorRelativeScale3D(FVector(X + 0.05f, 1.f, 1.f));
		}
	}

	// TODO: 2d array
	if (IdxOpenBuilding >= 0 && IdxOpenBuilding < ActorsChangingScale3D.Num())
	{
		FVector Scale = ActorsChangingScale3D[0]->GetActorScale();	// Temp
		if (FMath::IsNearlyEqual(Scale.X, 1.0, 1e-2))
		{
			IdxOpenBuilding = -1;
		}
		else
		{
			IsAllEnd = false;
			for (int i = 0; i < ActorsChangingScale3D.Num(); ++i)
			{
				ActorsChangingScale3D[i]->SetActorHiddenInGame(false);
				ActorsChangingScale3D[i]->SetActorRelativeScale3D(Scale + 0.05f);
			}
		}
	}

	if (IsAllEnd)
	{
		ElapsedEnv += _DeltaTime;
		if (ElapsedEnv > ENV_SEC)
		{
			ElapsedEnv = 0.f;
			ChangeState(EWorldState::ShowSavedLoc);
		}
	}
}

void AWorldManager::ChangeCameraToWorld()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr)
	{
		return;
	}

	AWorldPlayerController* PC = Cast<AWorldPlayerController>(Controller);
	if (PC == nullptr)
	{
		return;
	}

	PC->Server_SetViewTarget(WorldCamera);
}

void AWorldManager::ChangeCameraToPlayer()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APawn* PlayerPawn = PC->GetPawn();
		if (PlayerPawn)
		{
			PC->SetViewTargetWithBlend(PlayerPawn, 0.0f);
		}
	}
}

void AWorldManager::ToggleAllPlayersController(bool _IsOn)
{
	UWorld* Level = GetWorld();
	if (Level == nullptr)
	{
		return;
	}

	FConstPlayerControllerIterator iter = Level->GetPlayerControllerIterator();
	for (; iter; ++iter)
	{
		APlayerController* PC = Cast<APlayerController>(iter->Get());
		if (PC == nullptr)
		{
			continue;
		}

		APawn* PlayerPawn = PC->GetPawn();
		if (PlayerPawn == nullptr)
		{
			continue;
		}

		AWorldPlayer* Player = Cast<AWorldPlayer>(PlayerPawn);
		if (Player == nullptr)
		{
			continue;
		}

		Player->ToggleController(_IsOn);
	}
}

void AWorldManager::SetPlayersToSpectators()
{
	if (HasAuthority())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = It->Get();
			if (PC && !PC->IsLocalController())
			{
				PC->ChangeState(NAME_Spectating);
			}
		}
	}
}