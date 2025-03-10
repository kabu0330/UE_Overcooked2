// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2Global.h"
#include "Overcooked2.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Global/OC2GameInstance.h"
#include "LevelContent/Cook/Ingredient.h"
#include "Global/GameMode/CookingGameMode.h"
#include "Global/GameFramework/OC2Actor.h"
#include "Kismet/GameplayStatics.h"

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
	//FString OpenLevel;
	//FString LevelPath = TEXT("");

	//UOC2Global::GetAssetPackageName(UWorld::StaticClass(), LevelName, LevelPath);
	//OpenLevel = FString::Printf(TEXT(":%s%s"), *Port, *LevelPath);

	//UGameplayStatics::OpenLevel(World, *OpenLevel, true, TEXT("listen"));

	UOC2Global::GetOC2GameInstance(World)->CreateRoom();
}

//void UOC2Global::StartServer_Implementation(const UWorld* World, const FString& Port, const FString& LevelName)
//{
//	StartServer(World, Port, LevelName);
//
//}

void UOC2Global::ConnectServer(const UWorld* World, APlayerController* Controller, const FString& IP, const FString& Port, const FString& LevelName)
{
	//FString ConnectLevelName = FString::Printf(TEXT("%s:%s/%s"), *IP, *Port, *LevelName);
	////UGameplayStatics::OpenLevel(World, FName(*ConnectLevelName));

	//if (Controller != nullptr)
	//{
	//	Controller->ClientTravel(ConnectLevelName, ETravelType::TRAVEL_Absolute);
	//}

	UOC2Global::GetOC2GameInstance(World)->JoinRoom(IP, Controller);
}

void UOC2Global::TravelServer(UWorld* World, const FString& LevelName)
{
	//FString LevelPath = TEXT("");
	//UOC2Global::GetAssetPackageName(UWorld::StaticClass(), LevelName, LevelPath);

	//World->ServerTravel(LevelPath + TEXT("?listen"));

	UOC2Global::GetOC2GameInstance(World)->StartGame();
}

AOC2Actor* UOC2Global::SpawnOC2Actor(UWorld* World)
{
	ACookingGameMode* CookingGameMode = Cast<ACookingGameMode>(World->GetAuthGameMode());

	if (nullptr == CookingGameMode)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Current Game mode is not a cooking game mode"));
		return nullptr;
	}

	return CookingGameMode->SpawnOC2Actor();
}

AIngredient* UOC2Global::SpawnIngredientActor(UWorld* World, EIngredientType IngredientType)
{
	AGameModeBase* TestGameMode = World->GetAuthGameMode();
	ACookingGameMode* CookingGameMode = Cast<ACookingGameMode>(TestGameMode);

	if (nullptr == CookingGameMode)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Current Game mode is not a cooking game mode"));
		return nullptr;
	}

	return CookingGameMode->SpawnIngredientActor(IngredientType);
}

APlate* UOC2Global::SpawnPlateActor(UWorld* World)
{
	ACookingGameMode* CookingGameMode = Cast<ACookingGameMode>(World->GetAuthGameMode());

	if (nullptr == CookingGameMode)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Current Game mode is not a cooking game mode"));
		return nullptr;
	}

	return CookingGameMode->SpawnPlateActor();
}
