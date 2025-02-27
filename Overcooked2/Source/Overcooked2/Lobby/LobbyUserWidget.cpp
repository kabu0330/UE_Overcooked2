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