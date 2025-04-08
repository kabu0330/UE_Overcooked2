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

	if (true == bProgress)
	{
		CurTime += DeltaTime;

		if (CurTime > UOC2Const::OrderSpawnDelay)
		{
			if (OrderArray.Num() < UOC2Const::MaxOrderCount)
			{
				FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(GetWorld(), UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), OrderNumberArray[CurOrderIndex++]);
				CookingGameState->Multicast_CreateNewOrder(Order);
				OrderArray.Add(Order);

				if (CurOrderIndex == OrderNumberArray.Num())
				{
					CurOrderIndex = 0;
				}
			}

			CurTime = 0.0f;
		}

	}
}

void AStageManager::AddScore(int InScore)
{
	Score += InScore;
}

void AStageManager::CompleteOrder(FOrder Order)
{
	int OrderIndex = FindOrderIndex(Order);
	if (OrderIndex != -1)
	{
		CookingGameState->Multicast_CompleteOrder(OrderIndex);
	}
	else
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Order Index is a -1"));
	}
}

int AStageManager::FindOrderIndex(FOrder& Order)
{
	for (int i = 0; i < OrderArray.Num(); i++)
	{
		if (Order.OrderTexutre == OrderArray[i].OrderTexutre)
		{
			OrderArray.RemoveAt(i);
			return i;
		}
	}

	return -1;
}