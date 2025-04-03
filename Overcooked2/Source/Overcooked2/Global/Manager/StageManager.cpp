// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Manager/StageManager.h"
#include "Overcooked2.h"

#include "Global/GameMode/CookingGameMode.h"
#include "Global/State/GameState/CookingGameState.h"
#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2GameInstance.h"

AStageManager::AStageManager()
{
	PrimaryActorTick.bCanEverTick = true;

	int a = 0;

	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);

}

AStageManager* AStageManager::Get(UWorld* World)
{
	if (ACookingGameMode* CookingGameMode = Cast<ACookingGameMode>(World->GetAuthGameMode()))
	{
		return CookingGameMode->StageManager;
	}

	return nullptr;
}

void AStageManager::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == CookingGameState)
	{
		CookingGameState = GetWorld()->GetGameState<ACookingGameState>();
	}
}

void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurTime += DeltaTime;
	
	if (CurTime > UOC2Const::OrderSpawnDelay)
	{
		FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(GetWorld(), UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), OrderNumberArray[CurOrderIndex++]);
		CookingGameState->Multicast_CreateNewOrder(Order);

		if (CurOrderIndex == OrderNumberArray.Num())
		{
			CurOrderIndex = 0;
		}

		CurTime = 0.0f;
	}

}

void AStageManager::AddScore(int InScore)
{
	Score += InScore;
}
