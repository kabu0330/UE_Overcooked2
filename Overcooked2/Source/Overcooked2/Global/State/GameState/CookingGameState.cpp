// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/CookingGameState.h"
#include "Overcooked2.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"

#include "LevelContent/Cook/Plate.h"
#include "LevelContent/Cook/Cooking.h"
#include "LevelContent/Map/OC2Map.h"

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Global/GameMode/CookingGameMode.h"
#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2GameInstance.h"
#include "Global/Manager/StageManager.h"

#include "Character/OC2Character.h"
#include "Character/OC2CameraActor.h"

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

	for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
	{
		DirectionalLight = *It;
	}

	for (TActorIterator<AOC2Map> It(GetWorld()); It; ++It)
	{
		OC2Map = *It;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOC2CameraActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		CameraActor = Cast<AOC2CameraActor>(FoundActors[0]);
		if (CameraActor)
		{
			// 이제 CameraActor를 사용할 수 있음
			UE_LOG(LogTemp, Log, TEXT("Found AOC2CameraActor: %s"), *CameraActor->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No AOC2CameraActor found in level"));
	}

	if (true == HasAuthority())
	{
		int a = 0;
	}
	else
	{
		int a = 0;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());
		CookingHUD->InitCookWidget();

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->ShowHoldCanvas();
		}
	}
}

void ACookingGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACookingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookingGameState, CurStatgeState);
	DOREPLIFETIME(ACookingGameState, OrderScore);
	DOREPLIFETIME(ACookingGameState, FeverScore);
	DOREPLIFETIME(ACookingGameState, FailScore);
	DOREPLIFETIME(ACookingGameState, TotalScore);
}

void ACookingGameState::AddPlate(APlate* Plate)
{
	if (false == HasAuthority())
	{
		return;
	}
	if (true == PlateArray.Contains(Plate))
	{
		return;
	}
	PlateArray.Add(Plate);

	if (true == HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Server AddPlate : %d"), PlateArray.Num()));
	}
	if (false == HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Client AddPlate : %d"), PlateArray.Num()));
	}

}

APlate* ACookingGameState::GetPlate(int Index)
{
	if (false == HasAuthority())
	{
		return nullptr;
	}

	if (PlateArray.Num() > 0)
	{
		APlate* LastPlate = PlateArray.Last();
		PlateArray.Pop();

		if (true == HasAuthority())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Server GetPlate : %d"), PlateArray.Num()));
		}
		if (false == HasAuthority())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Client GetPlate : %d"), PlateArray.Num()));
		}

		return LastPlate;
	}
	else
	{
		UE_LOG(OVERCOOKED_LOG, Display, TEXT("PlateArray is empty!"));
		return nullptr;
	}
}

int ACookingGameState::GetOrderScore()
{
	return OrderScore;
}

int ACookingGameState::GetFeverScore()
{
	return FeverScore;
}

int ACookingGameState::GetFailScore()
{
	return FailScore;
}

int ACookingGameState::GetTotalScore()
{
	return TotalScore;
}

int ACookingGameState::GetPlateArrayNum()
{
	if (true == HasAuthority())
	{
		return PlateArray.Num();
	}

	return -1;
}

void ACookingGameState::OnPlayerCookingWidgetReady()
{
	if (true == HasAuthority())
	{
		ReadyPlayer.Add(0);

		if (ReadyPlayer.Num() == UOC2Global::GetOC2GameInstance(GetWorld())->GetUserCount())
		{
			//TODO: 게임모드 상태 변경
			ACookingGameMode* CookingGameMode = Cast<ACookingGameMode>(GetWorld()->GetAuthGameMode());
			if (nullptr != CookingGameMode)
			{
				CookingGameMode->ChangeState(ECookingGameModeState::ECS_Stay);
			}
		}
	}
}

void ACookingGameState::Multicast_CompleteOrder_Implementation(int OrderIndex, int InScore)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			if (-1 != OrderIndex)
			{
				CookingHUD->CookWidget->OrderComplete(OrderIndex, InScore);
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

void ACookingGameState::Multicast_SettingTimer_Implementation(float DeltaTime)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->StartTimerTick(DeltaTime);

			//if (true == CookingHUD->CookWidget->IsShowScoreWidget())
			//{
			//	ULightComponent* BaseLightComp = DirectionalLight->GetLightComponent();

			//	// DirectionalLightComponent로 캐스팅
			//	if (UDirectionalLightComponent* DirLightComp = Cast<UDirectionalLightComponent>(BaseLightComp))
			//	{
			//		DirLightComp->SetIntensity(5.0f); // 강도 줄이기
			//	}
			//}
		}
	}
}

void ACookingGameState::Server_SubmitPlate_Implementation(ACooking* Cooking)
{
	if (true == HasAuthority())
	{
		APlate* Plate = Cast<APlate>(Cooking);

		if (nullptr == Plate)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate is nullptr"));
			return;
		}

		FOrder CheckOrder;

		for (int i = 0; i < Plate->GetIngredients().Num(); i++)
		{
			FCookableIngredient CookableIngredient;

			CookableIngredient.IngredientState = Plate->GetIngredient(i).IngredientState;
			CookableIngredient.IngredientType = Plate->GetIngredient(i).IngredientType;

			CheckOrder.RequireIngredients.Add(CookableIngredient);
		}

		int FindIndex = AStageManager::Get(GetWorld())->FindOrderIndex(CheckOrder);

		if(-1 != FindIndex)
		{
			int InScore = CheckOrder.RequireIngredients.Num() * UOC2Const::ScoreValue;

			OrderScore += InScore;

			if (FeverCount == 0)
			{
				InScore += UOC2Const::TipValue;
				FeverScore += UOC2Const::TipValue;
			}
			else
			{
				InScore += UOC2Const::TipValue * FeverCount;
				FeverScore += UOC2Const::TipValue * FeverCount;
			}

			TotalScore += InScore;

			AStageManager::Get(GetWorld())->CompleteOrderByIndex(FindIndex, InScore);

			if (0 == FindIndex)
			{
				FeverCount++;
			}
			else
			{
				FeverCount = 0;
			}

			Multicast_SetFeverUI(FeverCount);
		}
		else
		{
			FailScore += 20;
			TotalScore -= 20;

			Multicast_BlinkOrderUI();
		}

		Plate->Multicast_SubmitPlate();
	}
}

void ACookingGameState::Multicast_SetFeverUI_Implementation(int InFeverCount)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			if (0 == InFeverCount)
			{
				InFeverCount = 1;
			}
			CookingHUD->CookWidget->CheckFeverTime(InFeverCount);
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

void ACookingGameState::Server_MovePlate_Implementation(ACooking* Cooking)
{
	if (true == HasAuthority())
	{
		APlate* Plate = Cast<APlate>(Cooking);

		if (nullptr == Plate)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate is nullptr"));
			return;
		}

		UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(GetWorld());

		if (nullptr == GameInstance)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("GameInstance is nullptr"));
			return;
		}

		ACookingGameMode* GameMode = Cast<ACookingGameMode>(UGameplayStatics::GetGameMode(this));

		if (nullptr != GameMode)
		{
			Plate->Multicast_MovePlate();
		}
	}
}

void ACookingGameState::Multicast_PlayGameMapSound_Implementation()
{
	OC2Map->PlaySound();
}

void ACookingGameState::Multicast_ShowTimesUpUI_Implementation()
{
	// TODO : Times Up UI를 띄우는 함수
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->ShowTimesUPAnim();
		}
	}
}

void ACookingGameState::Multicast_ShowScorePanemUI_Implementation()
{
	// TODO : Score Panel UI를 띄우는 함수
	if (nullptr != CameraActor)
	{
		CameraActor->SetCameraScoreLocation();
	}
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->ShowReceiptWidget();
		}
	}
}

void ACookingGameState::Multicast_SetCharacterActive_Implementation(bool bActive)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AOC2Character* PlayerCharacter = Cast<AOC2Character>(PlayerController->GetPawn());

	PlayerCharacter->SetMoveEnabled(bActive);
}

void ACookingGameState::Muticast_EndGame_Implementation()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly Mode;
	PlayerController->SetInputMode(Mode);
	PlayerController->SetShowMouseCursor(false);
}

void ACookingGameState::Multicast_SetCharacterHead_Implementation()
{
	UOC2GameInstance* OC2GameInstance = UOC2Global::GetOC2GameInstance(GetWorld());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AOC2Character* PlayerCharacter = Cast<AOC2Character>(PlayerController->GetPawn());

	PlayerCharacter->SetCharacterName(OC2GameInstance->GetChefHeadName());

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