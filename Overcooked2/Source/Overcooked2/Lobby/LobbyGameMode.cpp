// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"
#include "Overcooked2.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	PlayerControllers.Push(NewPlayerController);

	if (GetWorld()->GetAuthGameMode())
	{
		UE_LOG(OVERCOOKED_LOG, Warning, TEXT("Server Player %s has joined the game! Player count is %d"), *NewPlayerController->GetName(), PlayerControllers.Num());
	}
	else
	{
		//UE_LOG(OVERCOOKED_LOG, Warning, TEXT("Client Player %s has joined the game!"), *NewPlayerController->GetName());
	}

	if (4 == PlayerControllers.Num() && ELobbyGameModeState::ELS_Stay == CurState)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Player controller count is 4!!!!!!!!!!!!!!!!!!!!!!"), *NewPlayerController->GetName());
	}
}

void ALobbyGameMode::EntryStay()
{
}

void ALobbyGameMode::Stay(float DeltaTime)
{
}
