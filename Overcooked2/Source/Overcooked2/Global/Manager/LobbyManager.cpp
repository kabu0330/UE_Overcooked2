// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Manager/LobbyManager.h"

#include "Character/OC2Character.h"
#include "Global/State/GameState/LobbyGameState.h"
#include "Global/OC2GameInstance.h"
#include "Global/OC2Global.h"

ALobbyManager::ALobbyManager()
{

}

ALobbyManager* ALobbyManager::Get(UWorld* WorldContext)
{
	if (ALobbyGameState* LobbyGameState = WorldContext->GetGameState<ALobbyGameState>())
	{
		return LobbyGameState->LobbyManager;
	}

	return nullptr;
}

void ALobbyManager::BeginPlay()
{
	Super::BeginPlay();

	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();

	if (nullptr != LobbyGameState)
	{
		LobbyGameState->Multicast_UpdateUserPanelUI(UOC2Global::GetOC2GameInstance(GetWorld())->GetUserIndex());
	}
}

void ALobbyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}