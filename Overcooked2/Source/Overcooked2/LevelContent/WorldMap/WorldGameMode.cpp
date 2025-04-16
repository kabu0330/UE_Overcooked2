// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
#include "Overcooked2.h"

#include "Global/State/GameState/WorldGameState.h"

#include "LevelContent/WorldMap/WorldManager.h"
#include "LevelContent/WorldMap/WorldPlayer.h"
#include "Kismet/GameplayStatics.h"

AWorldGameMode::AWorldGameMode()
{
}

void AWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("WorldManager"), Actors);

	if (Actors.Num() > 0)
	{
		WorldManager = Cast<AWorldManager>(Actors[0]);
	}
}

void AWorldGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldGameMode::InitGameState()
{
	Super::InitGameState();

	AWorldGameState* WorldGameState = GetGameState<AWorldGameState>();

	if (nullptr == WorldGameState)
	{
		UE_LOG(OVERCOOKED_LOG, Log, TEXT("WorldGameState가 nullptr입니다."));
		return;
	}
}

void AWorldGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	PlayerControllers.Push(NewPlayerController);

	FInputModeGameOnly Mode;

	NewPlayerController->SetInputMode(Mode);
	NewPlayerController->SetShowMouseCursor(false);

	AWorldGameState* WorldGameState = GetGameState<AWorldGameState>();

	WorldGameState->Server_AddUserCount();
}
