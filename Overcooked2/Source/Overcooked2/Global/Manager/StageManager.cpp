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
				FOrder Order = UOC2GlobalData::GetOrderByStageAndIndex(
					GetWorld(), 
					UOC2Global::GetOC2GameInstance(GetWorld())->GetCurStage(), 
					OrderNumberArray[CurOrderIndex++]);

				CookingGameState->Multicast_CreateNewOrder(Order);
				OrderArray.Add(Order);

				if (CurOrderIndex == OrderNumberArray.Num())
				{
					CurOrderIndex = 0;
				}
			}

			CurTime = 0.0f;

			UE_LOG(OVERCOOKED_LOG, Log, TEXT("CurOrderIndex : %d"), CurOrderIndex);
		}

	}
}

void AStageManager::AddScore(int InScore)
{
	Score += InScore;
}

int AStageManager::CompleteOrder(FOrder Order, int InScore)
{
	int OrderIndex = FindOrderIndex(Order);
	if (OrderIndex != -1)
	{
		CookingGameState->Multicast_CompleteOrder(OrderIndex, InScore);
		OrderArray.RemoveAt(OrderIndex);
		return OrderIndex;
	}
	else
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Order Index is a -1"));
		return OrderIndex;
	}
}

int AStageManager::CompleteOrderByIndex(int OrderIndex, int InScore)
{
	CookingGameState->Multicast_CompleteOrder(OrderIndex, InScore);
	OrderArray.RemoveAt(OrderIndex);
	return OrderIndex;
}

int AStageManager::FindOrderIndex(FOrder& Order)
{
	for (int i = 0; i < OrderArray.Num(); i++)
	{
		const TArray<FCookableIngredient>& A = OrderArray[i].RequireIngredients;
		const TArray<FCookableIngredient>& B = Order.RequireIngredients;

		// 개수가 다르면 비교할 필요 없음
		if (A.Num() != B.Num())
		{
			continue;
		}

		TArray<FCookableIngredient> TempB = B; // 매칭된 재료 제거용
		bool bAllMatch = true;

		for (const FCookableIngredient& IngredientA : A)
		{
			bool bFound = false;

			for (int j = 0; j < TempB.Num(); j++)
			{
				if (TempB[j].IngredientType == IngredientA.IngredientType &&
					TempB[j].IngredientState == IngredientA.IngredientState)
				{
					TempB.RemoveAt(j);
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				bAllMatch = false;
				break;
			}
		}

		if (bAllMatch)
		{
			return i; // 완전 매칭!
		}
	}

	return -1; // 매칭된 오더 없음
}
