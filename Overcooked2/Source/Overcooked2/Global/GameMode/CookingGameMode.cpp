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
}

void ACookingGameMode::BeginPlay()
{
	Super::BeginPlay();

	CookingGameState = GetGameState<ACookingGameState>();

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

	UE_LOG(LogTemp, Warning, TEXT("Player %s has joined the game!"), *NewPlayerController->GetName());

	PlayerControllers.Push(NewPlayerController);

	if (4 == PlayerControllers.Num() && ECookingGameModeState::ECS_Stay == CurState)
	{
		ChangeState(ECookingGameModeState::ECS_Stage);
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

	//if (TestTime >= 5.0f)
	//{
	//	ChangeState(ECookingGameModeState::ECS_Score);
	//}
}

void ACookingGameMode::EntryStage()
{
	//for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	//{
	//	APlayerController* PC = It->Get();
	//	if (PC)
	//	{
	//		// Owning Pawn 가져오기
	//		AOC2Character* PlayerPawn = Cast< AOC2Character>(PC->GetPawn());

	//		if (PlayerPawn)
	//		{
	//			UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(PC->GetGameInstance());

	//			if (GameInstance != nullptr)
	//			{
	//				UE_LOG(LogTemp, Log, TEXT("GameInstance 가져오기 성공!"));

	//				PlayerPawn->SetCharacterName(GameInstance->GetChefHeadName());
	//			}

	//			UE_LOG(LogTemp, Log, TEXT("Found PlayerPawn: %s"), *PlayerPawn->GetName());
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("PlayerController does not have a Pawn"));
	//		}
	//	}
	//}

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

			//OrderManager->Multicast_CreateNewOrder(Order);

			CookingGameState->Multicast_CreateNewOrder(Order);
		}


		//FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(GetWorld(), UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), 1);

		//OrderManager->Multicast_CreateNewOrder(Order);
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

		//FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(GetWorld(), UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), 1);
		//OrderManager->Multicast_CompleteOrder(Order);
		//OrderManager->Multicast_BlinkOrderUI();
		//CheckTime = 0.0f;
	}
}

void ACookingGameMode::EntryScore()
{
	UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	EIngredientType Test = GameInstance->GetIngredientType(TEXT("Tomato"));
	const TArray<FIngredientCookDataRow> DataRows = GameInstance->GetIngredientCookDataRows(TEXT("Tomato"));

	UE_LOG(LogTemp, Log, TEXT("IngredientType: %d"), (int32)Test);
	UE_LOG(LogTemp, Log, TEXT("PrevState: %d"), (int32)DataRows.Num());
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
