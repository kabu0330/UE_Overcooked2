// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
#include "Overcooked2.h"
#include "Kismet/GameplayStatics.h"

#include "Global/State/GameState/WorldGameState.h"
#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"

#include "LevelContent/WorldMap/WorldManager.h"
#include "LevelContent/WorldMap/WorldPlayer.h"
#include "LevelContent/WorldMap/WorldGameMode.h"

#include "UI/WorldMap/WorldMapHUD.h"
#include "UI/WorldMap/UI/WorldMapUserWidget.h"
#include "UI/Loading/LoadingWidget.h"

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

	AWorldMapHUD* WorldMapHUD = Cast<AWorldMapHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (WorldMapHUD->WorldMapUserWidget != nullptr && WorldMapHUD != nullptr)
	{
		WorldMapHUD->WorldMapUserWidget->SettingDebugMessage(TEXT("World GameMode Tick"));
	}

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
