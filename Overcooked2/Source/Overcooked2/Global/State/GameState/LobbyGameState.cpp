// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/LobbyGameState.h"
#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"
#include "Global/Manager/LobbyManager.h"
#include "Global/Data/OC2GlobalData.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"

#include "Lobby/LobbyHUD.h"
#include "Lobby/LobbyUserWidget.h"
#include "Lobby/LobbyGameMode.h"

#include "Character/OC2Character.h"

#include "UI/Lobby/LobbyZoomInWidget.h"
#include "UI/Loading/LoadingWidget.h"
#include "UI/Lobby/LobbyChalkBoard.h"


ALobbyGameState::ALobbyGameState()
{
	PrimaryActorTick.bCanEverTick = true;     // 틱 가능하게
	PrimaryActorTick.bStartWithTickEnabled = true; // 시작 시부터 틱 켜기
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	//InitCharacter();
}

void ALobbyGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());

	//if (LobbyHUD->LobbyZoomInWidget != nullptr && LobbyHUD != nullptr)
	//{
	//	if (true == LobbyHUD->LoadingWidget->GetIsConnecting())
	//	{
	//		bChecked = true;
	//		Server_NotifyLoadingComplete();
	//	}
	//}
}

ALobbyManager* ALobbyGameState::GetLobbyManager() const
{
	return LobbyManager;
}

void ALobbyGameState::Multicast_UpdateUserPanelUI_Implementation(int Index)
{
	for (TActorIterator<ALobbyChalkBoard> It(GetWorld()); It; ++It)
	{
		ALobbyChalkBoard* FoundActor = *It;
		if (FoundActor)
		{
			if (UUserWidget* Widget = FoundActor->GetWidgetComponent()->GetUserWidgetObject())
			{
				if (ULobbyUserWidget* LobbyWidget = Cast<ULobbyUserWidget>(Widget))
				{
					for (int i = 0; i <= Index; i++)
					{
						UTexture2D* ChefTexture = UOC2GlobalData::GetChefTextureByIndex(GetWorld(), i);
						LobbyWidget->SetUserTexture(ChefTexture, i);
					}
				}
			}
			break;
		}
	}
}

void ALobbyGameState::Server_SpawnClientCharacter_Implementation(FVector SpawnLocation, const FString& ChefHeadName)
{
	if (true == HasAuthority())
	{
		if (nullptr != OC2CharacterClass)
		{
			AOC2Character* OC2Character = GetWorld()->SpawnActor<AOC2Character>(OC2CharacterClass, FVector::ZeroVector, FRotator::ZeroRotator);
			OC2Character->SetActorLocation(SpawnLocation);
			OC2Character->SetActorRotation(UOC2Const::TitleCharacterSpawnRotation);
			OC2Character->SetCharacterName(ChefHeadName);
			OC2Character->Chopping(true);
		}
	}
}

void ALobbyGameState::Server_NotifyLoadingComplete_Implementation()
{
	if (true == HasAuthority())
	{
		ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));

		if (nullptr != GameMode)
		{
			CompleteArray.Add(0);

			if (CompleteArray.Num() == GameMode->GetUserCount())
			{
				UOC2Global::TravelServer(GetWorld(), PLAY_LEVEL);
			}
		}
	}
}

void ALobbyGameState::InitCharacter()
{
	if (nullptr != OC2CharacterClass)
	{
		AOC2Character* OC2Character = GetWorld()->SpawnActor<AOC2Character>(OC2CharacterClass, FVector::ZeroVector, FRotator::ZeroRotator);
		OC2Character->SetActorLocation(UOC2Const::TitleCharacterSpawnLocation);
		int UserIndex = UOC2Global::GetOC2GameInstance(GetWorld())->GetUserIndex();
		OC2Character->AddActorWorldOffset(FVector(0.0f, UserIndex * UOC2Const::OC2CharacterSizeY, 0.0f));
		OC2Character->SetActorRotation(UOC2Const::TitleCharacterSpawnRotation);
		OC2Character->SetCharacterName(UOC2Global::GetOC2GameInstance(GetWorld())->GetChefHeadName());
		OC2Character->Chopping(true);
	}
}

void ALobbyGameState::UpdateChefTexture(int Index)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());

	if (LobbyHUD->LobbyWidget != nullptr && LobbyHUD != nullptr)
	{
		for (int i = 0; i < Index; i++)
		{
			UTexture2D* ChefTexture = UOC2GlobalData::GetChefTextureByIndex(GetWorld(), Index);
			LobbyHUD->LobbyWidget->SetUserTexture(ChefTexture, Index);
		}
	}
}

void ALobbyGameState::CheckClinetLoadingComplete()
{
	if (true == HasAuthority())
	{
		ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));

		if (nullptr != GameMode)
		{
			CompleteArray.Add(0);

			if (CompleteArray.Num() == GameMode->GetUserCount())
			{
				UOC2Global::TravelServer(GetWorld(), PLAY_LEVEL);
			}
		}
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

	if (LobbyHUD->LobbyZoomInWidget != nullptr && LobbyHUD != nullptr)
	{
		LobbyHUD->LobbyZoomInWidget->PlayZoomInAnimation();
	}
}