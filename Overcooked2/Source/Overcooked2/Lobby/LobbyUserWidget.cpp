// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyUserWidget.h"
#include "Global/OC2Global.h"
#include "Character/OC2Character.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void ULobbyUserWidget::MoveToPlayLevel()
{
	// Only server
	if (GetWorld()->GetAuthGameMode())
	{
		UOC2Global::TravelServer(GetWorld(), PLAY_LEVEL);
	}
}

void ULobbyUserWidget::StartServer()
{
	if (GetWorld()->GetAuthGameMode())
	{
		UOC2Global::StartServer(GetWorld(), Port, LOBBY_LEVEL);
	}
}

void ULobbyUserWidget::ConnectServer()
{
	APlayerController* Controller = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	UOC2Global::ConnectServer(GetWorld(), Controller, IP, Port, LOBBY_LEVEL);
}