// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/WorldGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "UI/WorldMap/WorldMapHUD.h"
#include "UI/WorldMap/UI/WorldMapUserWidget.h"
#include "UI/Loading/LoadingWidget.h"

#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"
#include "LevelContent/WorldMap/WorldGameMode.h"

AWorldGameState::AWorldGameState()
{
	PrimaryActorTick.bCanEverTick = true;     // 틱 가능하게
	PrimaryActorTick.bStartWithTickEnabled = true; // 시작 시부터 틱 켜기
}

void AWorldGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AWorldGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AWorldGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWorldGameState, CurUserCount);
}

void AWorldGameState::Server_AddUserCount_Implementation()
{
	if (true == HasAuthority())
	{
		CurUserCount++;
	}
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

#if UE_BUILD_SHIPPING
			UE_LOG(LogTemp, Warning, TEXT("CurUserCount: %d"), CurUserCount); // Shipping 빌드에서는 ifdef로 감싸기
#endif
			if (CompleteArray.Num() == UOC2Global::GetOC2GameInstance(GetWorld())->GetUserCount())
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
