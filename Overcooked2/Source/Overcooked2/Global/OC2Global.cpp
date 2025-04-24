// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2Global.h"
#include "Overcooked2.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"

#include "Global/OC2GameInstance.h"
#include "Global/GameMode/OC2GameMode.h"
#include "Global/GameFramework/OC2Actor.h"
#include "Global/State/GameState/CookingGameState.h"

#include "LevelContent/Cook/Ingredient.h"
#include "LevelContent/Cook/Cooking.h"
#include "LevelContent/Cook/Plate.h"


void UOC2Global::GetAssetPackageName(UClass* Class, const FString& AssetName, FString& Path)
{
	TArray<FAssetData> AllAssetData = GetAssetDataArray(Class);

	FName CheckAssetName = FName(AssetName);

	for (const FAssetData& AssetData : AllAssetData)
	{
		if (AssetData.AssetName == CheckAssetName)
		{
			Path = AssetData.PackageName.ToString();
			return;
		}
	}
}

TArray<FAssetData> UOC2Global::GetAssetDataArray(UClass* Class)
{
	TArray<FAssetData> MapList;


	if (false == FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		return MapList;
	}

	IAssetRegistry& AssetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	// Asset 상황에 대해서 아직 로드를 하지 않았는지 조건.
	if (true == AssetRegistry.IsLoadingAssets())
	{
		return MapList;
	}

	if (AssetRegistry.GetAssetsByClass(Class->GetClassPathName(), MapList))
	{
		return MapList;
	}

	return MapList;
}

UOC2GameInstance* UOC2Global::GetOC2GameInstance(const UWorld* World)
{
	return Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(World));
}

void UOC2Global::StartServer(const UWorld* World, const FString& Port, const FString& LevelName)
{
	UOC2Global::GetOC2GameInstance(World)->CreateRoom();
}

void UOC2Global::ConnectServer(const UWorld* World, APlayerController* Controller, const FString& IP, const FString& Port, const FString& LevelName)
{
	UOC2Global::GetOC2GameInstance(World)->JoinRoom(IP, Controller);
}

void UOC2Global::TravelServer(UWorld* World, const FString& LevelName)
{
	UOC2Global::GetOC2GameInstance(World)->StartGame();
}

void UOC2Global::StartCookingStage(UWorld* World)
{
	UOC2Global::GetOC2GameInstance(World)->StartGame();
}

void UOC2Global::SubmitPlate(UWorld* World, ACooking* Cooking)
{
	APlate* Plate = Cast<APlate>(Cooking);

	if (nullptr == Plate)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate is nullptr"));
		return;
	}

	ACookingGameState* GameState = Cast<ACookingGameState>(UGameplayStatics::GetGameState(World));

	if (nullptr == GameState)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("GameState is nullptr"));
		return;
	}

	GameState->Server_SubmitPlate(Plate);
}

void UOC2Global::MovePlate(UWorld* World, ACooking* Cooking)
{
	APlate* Plate = Cast<APlate>(Cooking);

	if (nullptr == Plate)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate is nullptr"));
		return;
	}

	ACookingGameState* GameState = Cast<ACookingGameState>(UGameplayStatics::GetGameState(World));

	if (nullptr == GameState)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("GameState is nullptr"));
		return;
	}

	GameState->Server_MovePlate(Plate);
}

APlate* UOC2Global::GetPlate(UWorld* World)
{
	ACookingGameState* GameState = Cast<ACookingGameState>(UGameplayStatics::GetGameState(World));

	if (nullptr == GameState)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("GameState is nullptr"));
		return nullptr;
	}

	return GameState->GetPlate(0);
}


void UOC2Global::CheckPlateToRecipe(UWorld* World, APlate* Plate)
{
	TArray<FRecipe> Recipes;

	for (int i = 0; i < Plate->GetIngredients().Num(); i++)
	{
		FRecipe Recipe;

		Recipe.IngredientState = Plate->GetIngredient(i).IngredientState;
		Recipe.IngredientType = Plate->GetIngredient(i).IngredientType;

		Recipes.Add(Recipe);
	}

	static FPlateInitData EmptyArray;
	TArray<FRecipeDataRow*> FindRecipes;
}

int UOC2Global::GetOrderScore(UWorld* World)
{
	ACookingGameState* CookingGameState = World->GetGameState<ACookingGameState>();

	if (nullptr != CookingGameState)
	{
		return CookingGameState->GetOrderScore();
	}

	return 0;
}

int UOC2Global::GetFeverScore(UWorld* World)
{
	ACookingGameState* CookingGameState = World->GetGameState<ACookingGameState>();

	if (nullptr != CookingGameState)
	{
		return CookingGameState->GetFeverScore();
	}

	return 0;
}

int UOC2Global::GetFailScore(UWorld* World)
{
	ACookingGameState* CookingGameState = World->GetGameState<ACookingGameState>();

	if (nullptr != CookingGameState)
	{
		return CookingGameState->GetFailScore();
	}

	return 0;
}

int UOC2Global::GetTotalScore(UWorld* World)
{
	ACookingGameState* CookingGameState = World->GetGameState<ACookingGameState>();

	if (nullptr != CookingGameState)
	{
		return CookingGameState->GetTotalScore();
	}

	return 0;
}

int UOC2Global::GetDeliveryCount(UWorld* World)
{
	ACookingGameState* CookingGameState = World->GetGameState<ACookingGameState>();

	if (nullptr != CookingGameState)
	{
		return CookingGameState->GetDeliveryCount();
	}

	return 0;
}

int UOC2Global::GetFailCount(UWorld* World)
{
	ACookingGameState* CookingGameState = World->GetGameState<ACookingGameState>();

	if (nullptr != CookingGameState)
	{
		return CookingGameState->GetFailCount();
	}

	return 0;
}
