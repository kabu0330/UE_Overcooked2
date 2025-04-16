// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/WorldGameState.h"

#include "Kismet/GameplayStatics.h"

#include "UI/WorldMap/WorldMapHUD.h"
#include "UI/WorldMap/UI/WorldMapUserWidget.h"
#include "UI/Loading/LoadingWidget.h"

#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"
#include "LevelContent/WorldMap/WorldGameMode.h"

AWorldGameState::AWorldGameState()
{
}

void AWorldGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AWorldGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AWorldGameState::CheckClinetLoadingComplete()
{
	if (true == HasAuthority())
	{
		AWorldGameMode* GameMode = Cast<AWorldGameMode>(UGameplayStatics::GetGameMode(this));

		if (nullptr != GameMode)
		{
			CompleteArray.Add(0);

			int Test = GameMode->GetUserCount();

			if (CompleteArray.Num() == GameMode->GetUserCount())
			{
				UOC2Global::GetOC2GameInstance(GetWorld())->StartCookingStage();
			}
		}
	}
}

void AWorldGameState::Multicast_PlayZoomInAnmationUI_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	AWorldMapHUD* WorldMapHUD = Cast<AWorldMapHUD>(PlayerController->GetHUD());

	if (WorldMapHUD->WorldMapUserWidget != nullptr && WorldMapHUD != nullptr)
	{
		WorldMapHUD->WorldMapUserWidget->PlayZoomInAnimation();
	}
}
