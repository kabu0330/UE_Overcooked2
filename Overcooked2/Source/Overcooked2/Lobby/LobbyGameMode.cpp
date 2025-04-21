// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"
#include "Overcooked2.h"

#include "Lobby/LobbyPlayerController.h"
#include "Global/State/GameState/LobbyGameState.h"
#include "Global/Manager/LobbyManager.h"

#include "Global/OC2GameInstance.h"
#include "Global/OC2Global.h"
#include "Global/OC2Const.h"

ALobbyGameMode::ALobbyGameMode()
{
	ChefHeadNames.Add(UOC2Const::ChefEagleHeadName);
	ChefHeadNames.Add(UOC2Const::ChefMouseHeadName);
	ChefHeadNames.Add(UOC2Const::ChefPandaHeadName);
	ChefHeadNames.Add(UOC2Const::ChefPigHeadName);
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyGameMode::InitGameState()
{
	Super::InitGameState();

	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();

	if (nullptr == LobbyGameState)
	{
		UE_LOG(OVERCOOKED_LOG, Log, TEXT("LobbyGameState가 nullptr입니다."));
		return;
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	PlayerControllers.Push(NewPlayerController);

	if (PlayerControllers.Num() == 1)
	{
		UOC2Global::GetOC2GameInstance(GetWorld())->SetUserIndex(0);
	}

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayerController);

	if (nullptr != LobbyPlayerController)
	{
		LobbyPlayerController->Client_SetChefHeadName(ChefHeadNames[CurIdx]);
		LobbyPlayerController->Client_SetUserIndex(CurUserCount);
		CurUserCount++;
		CurIdx++;
	}

	UOC2Global::GetOC2GameInstance(GetWorld())->SetUserCount(CurUserCount);

	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();

	if (nullptr != LobbyGameState)
	{
		LobbyGameState->Multicast_UpdateUserPanelUI(CurUserCount - 1);

		int UserIndex = CurUserCount - 1;

		LobbyGameState->Server_SpawnClientCharacter(
			UOC2Const::TitleCharacterSpawnLocation + FVector(0.0f, UserIndex * UOC2Const::OC2CharacterSizeY, 0.0f),
			ChefHeadNames[UserIndex]);
	}
}