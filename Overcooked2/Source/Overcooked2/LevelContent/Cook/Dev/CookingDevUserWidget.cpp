// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevUserWidget.h"
#include <LevelContent/Cook/Ingredient.h>
#include "Kismet/GameplayStatics.h"
#include <LevelContent/Cook/Dev/CookingDevGameMode.h>
#include <Global/OC2Global.h>
#include <LevelContent/Cook/Dev/CookingDevGameState.h>
#include <LevelContent/Cook/Dev/CookingDevHUD.h>
#include <levelcontent/Cook/Dev/SpawnManagerComponent.h>
#include <Character/OC2Character.h>
#include <LevelContent/Cook/Dev/CookingDevPlayerState.h>

void UCookingDevUserWidget::ServerSpawnIngredient(EIngredientType Type)
{

	// Pawn에서 네트워크 동기화 성공 HUD -> SpawnManager -> GameMode
	//APlayerController* Controller = GetOwningPlayer();
	//if (nullptr != Controller)
	//{
	//	AOC2Character* Pawn = Cast<AOC2Character>(Controller->GetPawn());
	//	//Pawn->SpawnManager->SetIngredientType(Type);
	//}

	// PlayerState에서 네트워크 동기화 성공
	APlayerController* Controller = GetOwningPlayer();
	ACookingDevPlayerState* PlayerState = Controller->GetPlayerState<ACookingDevPlayerState>();
	if (nullptr != PlayerState)
	{
		PlayerState->SpawnManagerComponent->SetIngredientType(Type);
	}
}

void UCookingDevUserWidget::SpawnPlate()
{
	APlayerController* Controller = GetOwningPlayer();
	ACookingDevPlayerState* PlayerState = Controller->GetPlayerState<ACookingDevPlayerState>();
	if (nullptr != PlayerState)
	{
		PlayerState->SpawnPlate();
	}
}

void UCookingDevUserWidget::ChangeState(EIngredientState State)
{
	APlayerController* Controller = GetOwningPlayer();
	ACookingDevPlayerState* PlayerState = Controller->GetPlayerState<ACookingDevPlayerState>();
	if (nullptr != PlayerState)
	{
		PlayerState->ChangeState(State);
	}
}

void UCookingDevUserWidget::PlaceOnthePlate()
{
	APlayerController* Controller = GetOwningPlayer();
	ACookingDevPlayerState* PlayerState = Controller->GetPlayerState<ACookingDevPlayerState>();
	if (nullptr != PlayerState)
	{
		PlayerState->PlaceOnthePlate();
	}
}

void UCookingDevUserWidget::StartServer()
{
	FString OpenLevel = TEXT("");
	FString LevelPath = TEXT(""); // 레벨의 폴더 경로를 저장할 변수

	UOC2Global::GetAssetPackageName(UWorld::StaticClass(), TEXT("CookingDevLevel"), LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *Port, *LevelPath);
	// :30000/Game/Map/CookingDevLevel

	UGameplayStatics::OpenLevel(GetWorld(), *OpenLevel, true, TEXT("listen"));
}

void UCookingDevUserWidget::Connect()
{
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *IP, *Port);
	// 127.0.0.1:30000

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ConnectLevelName));

	// Debug
	UE_LOG(LogTemp, Warning, TEXT("ConnectLevelName: %s"), *ConnectLevelName);
}
