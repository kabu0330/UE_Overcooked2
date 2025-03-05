// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameMode/CookingGameMode.h"
#include "Global/OC2GameInstance.h"
#include "Global/Data/IngredientDataTable.h"
#include "Global/Component/OrderManageComponent.h"
#include "Global/OC2Const.h"

#include "Kismet/GameplayStatics.h"

ACookingGameMode::ACookingGameMode()
{
	OrderManager = CreateDefaultSubobject<UOrderManageComponent>(TEXT("OrderManager"));
}

void ACookingGameMode::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(ECookingGameModeState::ECS_Stage);
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
}

void ACookingGameMode::Stay(float DeltaTime)
{
	//TestTime += DeltaTime;

	//if (TestTime >= 5.0f)
	//{
	//	ChangeState(ECookingGameModeState::ECS_Score);
	//}
}

void ACookingGameMode::EntryStage()
{
	CheckTime = 0.0f;
}

void ACookingGameMode::Stage(float DeltaTime)
{
	CheckTime += DeltaTime;

	if (CheckTime >= UOC2Const::OrderSpawnDelay)
	{
		CheckTime = 0.0f;
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
