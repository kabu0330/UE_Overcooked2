// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameMode/CookingGameMode.h"

#include "Global/OC2GameInstance.h"
#include "Global/GameFramework/OC2Actor.h"
#include "Global/Component/OrderManageComponent.h"
#include "Global/Component/SpawnManageComponent.h"
#include "Global/Data/IngredientDataTable.h"
#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2Global.h"
#include "Global/State/GameState/CookingGameState.h"
#include "Global/Manager/SpawnManager.h"
#include "Global/Manager/StageManager.h"

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Character/OC2Character.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"


ACookingGameMode::ACookingGameMode()
{
	OrderManager = CreateDefaultSubobject<UOrderManageComponent>(TEXT("OrderManager"));
}

void ACookingGameMode::BeginPlay()
{
	Super::BeginPlay();

	CookingGameState = GetGameState<ACookingGameState>();

	if (true == HasAuthority()) // 서버에서만 실행
	{
		SpawnManagerActor = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);
	}
	if (true == HasAuthority())
	{
		StageManager = GetWorld()->SpawnActor<AStageManager>(StageManagerSpawner);
	}

	CurIdx = 0;

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ACookingGameMode::StartStage,
		3.0f,   // 3초 뒤 실행
		false   // 반복 여부(false면 1회 실행)
	);
}

void ACookingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurState)
	{
	case ECookingGameModeState::ECS_Stay:
		Stay(DeltaTime);
		break;
	case ECookingGameModeState::ECS_Stage:
		Stage(DeltaTime);
		break;
	case ECookingGameModeState::ECS_TimeUp:
		TimeUp(DeltaTime);
		break;
	case ECookingGameModeState::ECS_Score:
		Score(DeltaTime);
		break;
	default:
		break;
	}
}

void ACookingGameMode::AddPlate(APlate* Plate)
{
	PlateArray.Add(Plate);
}

APlate* ACookingGameMode::GetPlate()
{
	if (PlateArray.Num() > 0)
	{
		APlate* LastPlate = PlateArray.Last();
		PlateArray.Pop();

		return LastPlate;
	}
	else
	{
		UE_LOG(OVERCOOKED_LOG, Display, TEXT("PlateArray is empty!"));
		return nullptr;
	}
}

void ACookingGameMode::EntryStay()
{
	CheckTime = 0.0f;

	CookingGameState->Multicast_SetCharacterActive(false);
	CookingGameState->Multicast_SetCharacterHead();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			if (nullptr != CookingGameState)
			{
				CookingGameState->Multicast_StartGame();
			}
		}
	}
}

void ACookingGameMode::Stay(float DeltaTime)
{
	CheckTime += DeltaTime;

	if (CheckTime >= 3.5f)
	{
		ChangeState(ECookingGameModeState::ECS_Stage);
	}
}

void ACookingGameMode::EntryStage()
{
	StageManager->bProgress = true;

	PlayBackgroundSound();

	CookingGameState->Multicast_SetCharacterActive(true);

	CheckTime = 0.0f;
}

void ACookingGameMode::Stage(float DeltaTime)
{
	GameTime -= DeltaTime;

	if (nullptr != CookingGameState)
	{
		CookingGameState->Multicast_SettingTimer(GameTime);
	}

	if (false == bShowTimesUpUI && GameTime <= 0.0f)
	{
		CookingGameState->Multicast_ShowTimesUpUI();
		bShowTimesUpUI = true;
		ChangeState(ECookingGameModeState::ECS_TimeUp);
	}
}

void ACookingGameMode::TimeUp(float DeltaTime)
{
	CheckTime += DeltaTime;
	
	if (CheckTime > 1.5f)
	{
		CookingGameState->Multicast_ShowScorePanemUI();
		ChangeState(ECookingGameModeState::ECS_Score);
	}
}

void ACookingGameMode::EntryTimeUp()
{
	StageManager->bProgress = false;
	CheckTime = 0.0f;

	UWorld* World = GetWorld();

	if (nullptr == World)
	{
		return;
	}

	CookingGameState->Multicast_SetCharacterActive(false);
}

void ACookingGameMode::EntryScore()
{
	CookingGameState->Muticast_EndGame();
}


void ACookingGameMode::Score(float DeltaTime)
{
}

void ACookingGameMode::ChangeState(ECookingGameModeState State)
{
	CurState = State;

	switch (CurState)
	{
	case ECookingGameModeState::ECS_Stay:
		EntryStay();
		break;
	case ECookingGameModeState::ECS_Stage:
		EntryStage();
		break;
	case ECookingGameModeState::ECS_TimeUp:
		EntryTimeUp();
		break;
	case ECookingGameModeState::ECS_Score:
		EntryScore();
		break;
	default:
		break;
	}
}

void ACookingGameMode::PrintDebugMessage()
{
	switch (CurState)
	{
	case ECookingGameModeState::ECS_Stay:
		UE_LOG(LogTemp, Warning, TEXT("GameMode State : Stay"));
		break;
	case ECookingGameModeState::ECS_Stage:
		UE_LOG(LogTemp, Warning, TEXT("GameMode State : Stage"));
		break;
	case ECookingGameModeState::ECS_Score:
		UE_LOG(LogTemp, Warning, TEXT("GameMode State : Score"));
		break;
	default:
		break;
	}
}

void ACookingGameMode::PlayBackgroundSound()
{
	CookingGameState->Multicast_PlayGameMapSound();
}

void ACookingGameMode::StartStage()
{
	ChangeState(ECookingGameModeState::ECS_Stay);
}
 