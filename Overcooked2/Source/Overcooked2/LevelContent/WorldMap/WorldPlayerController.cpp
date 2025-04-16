// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelContent/WorldMap/WorldPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Global/State/GameState/WorldGameState.h"

#include "UI/WorldMap/WorldMapHUD.h"
#include "UI/WorldMap/UI/WorldMapUserWidget.h"
#include "UI/Loading/LoadingWidget.h"

AWorldPlayerController::AWorldPlayerController()
{
}

void AWorldPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AWorldMapHUD* WorldMapHUD = Cast<AWorldMapHUD>(GetHUD());

	if (nullptr != WorldMapHUD)
	{
		if (nullptr != WorldMapHUD->LoadingWidget)
		{
			if (true == WorldMapHUD->LoadingWidget->GetIsConnecting() &&
				false == bReported)
			{
				bReported = true;
				Server_NotifyLoadingComplete();
			}
		}
	}
}

void AWorldPlayerController::AddInputMappingContext(UInputMappingContext* _Context)
{
	if (GetLocalPlayer() == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	InputSystem->ClearAllMappings();
	InputSystem->AddMappingContext(_Context, 0);
}

void AWorldPlayerController::Server_SetViewTarget_Implementation(AActor* NewCamera)
{
    if (NewCamera)
    {
        SetViewTargetWithBlend(NewCamera, 1.0f);
    }
}

void AWorldPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AWorldPlayerController::Server_NotifyLoadingComplete_Implementation()
{
	AWorldGameState* WorldGameState = GetWorld()->GetGameState<AWorldGameState>();

	if (nullptr != WorldGameState)
	{
		WorldGameState->CheckClinetLoadingComplete();
	}
}
