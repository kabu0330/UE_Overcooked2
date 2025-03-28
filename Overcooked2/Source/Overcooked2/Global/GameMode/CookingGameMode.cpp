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

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Character/OC2Character.h"

#include "Kismet/GameplayStatics.h"

ACookingGameMode::ACookingGameMode()
{
	OrderManager = CreateDefaultSubobject<UOrderManageComponent>(TEXT("OrderManager"));

	ChefHeadNames.Add(UOC2Const::ChefEagleHeadName);
	ChefHeadNames.Add(UOC2Const::ChefMouseHeadName);
	ChefHeadNames.Add(UOC2Const::ChefPandaHeadName);
	ChefHeadNames.Add(UOC2Const::ChefPigHeadName);
}

void ACookingGameMode::BeginPlay()
{
	Super::BeginPlay();

	CookingGameState = GetGameState<ACookingGameState>();

	CurIdx = 0;

	ChangeState(ECookingGameModeState::ECS_Stay);
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
	case ECookingGameModeState::ECS_Score:
		Score(DeltaTime);
		break;
	default:
		break;
	}
}

void ACookingGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);
}

void ACookingGameMode::InitChef()
{
	UWorld* World = GetWorld();

	if (nullptr == World)
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();

		if (nullptr != PlayerController)
		{
			ACharacter* DefaultCharacter = PlayerController->GetCharacter();

			if (nullptr != DefaultCharacter)
			{
				AOC2Character* OC2Character = Cast<AOC2Character>(DefaultCharacter);

				if (nullptr != OC2Character)
				{
					OC2Character->SetCharacterName(ChefHeadNames[CurIdx]);
					CurIdx++;
				}
			}
		}
	}
}

void ACookingGameMode::EntryStay()
{
	CheckTime = 0.0f;
}

void ACookingGameMode::Stay(float DeltaTime)
{
	CheckTime += DeltaTime;

	if (CheckTime > 3.0f)
	{
		ChangeState(ECookingGameModeState::ECS_Stage);
	}
}

void ACookingGameMode::EntryStage()
{
	InitChef();

	CheckTime = 0.0f;
}

void ACookingGameMode::Stage(float DeltaTime)
{
	CheckTime += DeltaTime;

	if (CheckTime >= UOC2Const::OrderSpawnDelay)
	{

		if (CookingGameState != nullptr)
		{
			FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(GetWorld(), UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), 1);
			CookingGameState->Multicast_CreateNewOrder(Order);
		}
	}

	if (CheckTime >= 6.0f)
	{
		if (CookingGameState != nullptr)
		{
			FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(GetWorld(), UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), 1);

			CookingGameState->Multicast_CompleteOrder(Order);
			CookingGameState->Multicast_BlinkOrderUI();

			CheckTime = 0.0f;

		}
	}
}

void ACookingGameMode::EntryScore()
{
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
