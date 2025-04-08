// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/CookingGameState.h"
#include "Overcooked2.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);

	ChangeState(ECookingStageState::ESS_WAITING_TO_START);

}

void ACookingGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurStatgeState)
	{
	case ECookingStageState::ESS_WAITING_TO_START:
		WaitingToStart(DeltaTime);
		break;
	case ECookingStageState::ESS_IN_PROGRESS:
		InProgress(DeltaTime);
		break;
	case ECookingStageState::ESS_WAITING_POST_MATCH:
		WaitingPostMatch(DeltaTime);
		break;
	}

}

void ACookingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookingGameState, CurStatgeState);
}

void ACookingGameState::ChangeState(ECookingStageState ChangeState)
{
	if (true == HasAuthority())
	{
		CurStatgeState = ChangeState;
		switch (CurStatgeState)
		{
		case ECookingStageState::ESS_WAITING_TO_START:
			EntryWaitingToStart();
			break;
		case ECookingStageState::ESS_IN_PROGRESS:
			EntryInProgress();
			break;
		case ECookingStageState::ESS_WAITING_POST_MATCH:
			EntryWaitingPostMatch();
			break;
		}
	}
}

void ACookingGameState::EntryWaitingToStart()
{
	SetMatchState(MatchState::WaitingToStart);
}

void ACookingGameState::WaitingToStart(float DeltaTime)
{
}

void ACookingGameState::EntryInProgress()
{
}

void ACookingGameState::InProgress(float DeltaTime)
{
	
}

void ACookingGameState::EntryWaitingPostMatch()
{
}

void ACookingGameState::WaitingPostMatch(float DeltaTime)
{
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

void ACookingGameState::Multicast_StartGame_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->StartGame();
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

			Plate->SubmitPlate();
		}
	}
}

void ACookingGameState::Multicast_AddScore_Implementation(int InScore)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	int Test = InScore;
	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->WrongOrder();
		}
	}
}

void ACookingGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();


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