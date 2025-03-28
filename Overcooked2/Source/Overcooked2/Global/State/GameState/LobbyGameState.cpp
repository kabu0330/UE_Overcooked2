// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/LobbyGameState.h"

#include "Global/Manager/LobbyManager.h"

#include "Net/UnrealNetwork.h"

ALobbyManager* ALobbyGameState::GetLobbyManager() const
{
	return LobbyManager;
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, LobbyManager);
}
