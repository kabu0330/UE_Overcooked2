// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/LobbyGameState.h"
#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"
#include "Global/Manager/LobbyManager.h"
#include "Global/Data/OC2GlobalData.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

#include "Lobby/LobbyHUD.h"
#include "Lobby/LobbyUserWidget.h"
#include "UI/Lobby/LobbyZoomInWidget.h"
#include "UI/Loading/LoadingWidget.h"


ALobbyGameState::ALobbyGameState()
{
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ALobbyManager* ALobbyGameState::GetLobbyManager() const
{
	return LobbyManager;
}

void ALobbyGameState::Multicast_UpdateUserPanelUI_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());

	
	if (LobbyHUD->LobbyWidget != nullptr && LobbyHUD != nullptr)
	{
		UTexture2D* ChefTexture = UOC2GlobalData::GetChefTexture(GetWorld());
		int UserIndex = UOC2Global::GetOC2GameInstance(GetWorld())->GetUserIndex();
		LobbyHUD->LobbyWidget->SetUserTexture(ChefTexture, UserIndex);
	}
}

void ALobbyGameState::UpdateChefTexture()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());


	if (LobbyHUD->LobbyWidget != nullptr && LobbyHUD != nullptr)
	{
		UTexture2D* ChefTexture = UOC2GlobalData::GetChefTexture(GetWorld());
		int UserIndex = UOC2Global::GetOC2GameInstance(GetWorld())->GetUserIndex();
		LobbyHUD->LobbyWidget->SetUserTexture(ChefTexture, UserIndex);
	}
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, LobbyManager);
}

void ALobbyGameState::Multicast_PlayZoomInAnmationUI_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());


	if (true == HasAuthority())
	{
		if (LobbyHUD->LobbyZoomInWidget != nullptr && LobbyHUD != nullptr)
		{
			LobbyHUD->LobbyZoomInWidget->PlayZoomInAnimation([this]()
				{
					UOC2Global::TravelServer(GetWorld(), PLAY_LEVEL);
				});
		}
	}
	else
	{
		if (LobbyHUD->LobbyZoomInWidget != nullptr && LobbyHUD != nullptr)
		{
			LobbyHUD->LobbyZoomInWidget->PlayZoomInAnimation([]() {});
		}
	}
}