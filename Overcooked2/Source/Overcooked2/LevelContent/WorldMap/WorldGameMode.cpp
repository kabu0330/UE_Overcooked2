// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
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

void AWorldGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	FInputModeGameOnly Mode;

	NewPlayerController->SetInputMode(Mode);
	NewPlayerController->SetShowMouseCursor(false);
}
