// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyHUD.h"
#include "UI/Lobby/LobbyZoomInWidget.h"
#include "UI/Loading/LoadingWidget.h"
#include "Lobby/LobbyUserWidget.h"

#include "Global/State/GameState/LobbyGameState.h"

#include "Overcooked2.h"


void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	LobbyWidget = Cast<ULobbyUserWidget>(CreateWidget(GetWorld(), LobbyWidgetObject));
	LobbyZoomInWidget = Cast<ULobbyZoomInWidget>(CreateWidget(GetWorld(), LobbyZoomInWidgetObject));
	LoadingWidget = Cast<ULoadingWidget>(CreateWidget(GetWorld(), LoadingWidgetObject));

	if (LobbyWidget == nullptr || LobbyZoomInWidget == nullptr || LoadingWidget == nullptr)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("ÄîÀ§Á¬ÀÌ nullptr ÀÔ´Ï´Ù!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	}

	LobbyZoomInWidget->AddToViewport();
	LoadingWidget->AddToViewport();
	LoadingWidget->SetVisibility(ESlateVisibility::Hidden);

	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();

	if (nullptr != LobbyGameState)
	{
		LobbyGameState->Multicast_UpdateUserPanelUI();
	}
}

void ALobbyHUD::ShowLoadingWidget()
{
	LoadingWidget->SetVisibility(ESlateVisibility::Visible);
	LoadingWidget->PlayZoomOutAnimation();
}