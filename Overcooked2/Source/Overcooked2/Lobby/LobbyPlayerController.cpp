// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "Global/OC2GameInstance.h"
#include "Global/Manager/LobbyManager.h"
#include "Global/State/GameState/LobbyGameState.h"
#include "Global/OC2Global.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

#include "UI/Lobby/LobbyZoomInWidget.h"
#include "UI/Loading/LoadingWidget.h"
#include "UI/Lobby/LobbyChalkBoard.h"
#include "Lobby/LobbyHUD.h"


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);

	SetShowMouseCursor(true);

	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();

	if (nullptr != LobbyGameState)
	{
		for (int i = 0; i <= UOC2Global::GetOC2GameInstance(GetWorld())->GetUserIndex(); i++)
		{
			LobbyGameState->Multicast_UpdateUserPanelUI(i);
		}
	}
}

void ALobbyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(GetHUD());

	if (LobbyHUD != nullptr)
	{
		if (LobbyHUD->LobbyZoomInWidget != nullptr)
		{
			if (true == LobbyHUD->LoadingWidget->GetIsConnecting() &&
				false == bReported)
			{
				bReported = true;
				Server_NotifyLoadingComplete();
			}
		}
	}
}

void ALobbyPlayerController::Server_NotifyLoadingComplete_Implementation()
{
	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();

	if (nullptr != LobbyGameState)
	{
		LobbyGameState->CheckClinetLoadingComplete();
	}
}

void ALobbyPlayerController::UpdateChefTexture_Implementation(int UserIndex)
{

}

void ALobbyPlayerController::Client_SetUserIndex_Implementation(int InUserIndex)
{
	UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetUserIndex(InUserIndex);
	}
}

// ALobbyPlayerController.cpp
void ALobbyPlayerController::Client_SetChefHeadName_Implementation(const FString& NewChefHeadName)
{
	UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetChefHeadName(NewChefHeadName);
	}
}