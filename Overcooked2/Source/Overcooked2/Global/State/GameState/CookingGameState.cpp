// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/CookingGameState.h"
#include "Overcooked2.h"

#include "Kismet/GameplayStatics.h"

#include "LevelContent/Cook/Plate.h"
#include "LevelContent/Cook/Cooking.h"

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Global/GameMode/CookingGameMode.h"
#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2GameInstance.h"
#include "Global/Manager/StageManager.h"

ACookingGameState::ACookingGameState()
{
}

void ACookingGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACookingGameState::Multicast_CompleteOrder_Implementation(int OrderIndex)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			if (-1 != OrderIndex)
			{
				CookingHUD->CookWidget->OrderComplete(OrderIndex);
			}
		}
	}
}

void ACookingGameState::Multicast_CreateNewOrder_Implementation(FOrder Order)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			FOrder CurOrder = Order;
			OrderList.Push(Order);
			CookingHUD->CookWidget->CreateNewOrder(Order);
		}
	}
}

void ACookingGameState::Multicast_BlinkOrderUI_Implementation()
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->WrongOrder();
		}
	}
}

void ACookingGameState::Server_SubmitPlate_Implementation(ACooking* Cooking)
{
	APlate* Plate = Cast<APlate>(Cooking);

	if (true == HasAuthority())
	{
		ACookingGameMode* GameMode = Cast<ACookingGameMode>(UGameplayStatics::GetGameMode(this));

		if (nullptr != GameMode)
		{
			FOrder Order;
			
			Order = UOC2GlobalData::GetOrderByIngredients(GetWorld(), Plate);
			
			GameMode->StageManager->CompleteOrder(Order);
		}
	}
}

void ACookingGameState::Multicast_AddScore_Implementation(int Score)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->WrongOrder();
		}
	}
}

int ACookingGameState::FindOrderIndex(FOrder& Order)
{
	for (int i = 0; i < OrderList.Num(); i++)
	{
		if (Order.OrderTexutre == OrderList[i].OrderTexutre)
		{
			return i;
		}
	}

	return -1;
}