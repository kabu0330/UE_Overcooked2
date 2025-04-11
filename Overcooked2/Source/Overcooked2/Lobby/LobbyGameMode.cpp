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

	//UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(GetGameInstance());

	//if (GameInstance != nullptr)
	//{
	//	GameInstance->SetChefHeadName(ChefHeadNames[CurIdx]);
	//	CurIdx++;
	//}

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

	//LobbyGameState->LobbyManager = GetWorld()->SpawnActor<ALobbyManager>(LobbyGameState->LobbyManagerClass);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	PlayerControllers.Push(NewPlayerController);

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayerController);
	if (nullptr != LobbyPlayerController)
	{
		LobbyPlayerController->Client_SetChefHeadName(ChefHeadNames[CurIdx]);
		LobbyPlayerController->Client_SetUserIndex(CurUserCount);
		CurUserCount++;
		CurIdx++;
	}

	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();

	if (nullptr != LobbyGameState)
	{
		LobbyGameState->Multicast_UpdateUserPanelUI(CurUserCount - 1);
	}
}

void ALobbyGameMode::EntryStay()
{
}

void ALobbyGameMode::Stay(float DeltaTime)
{
}
