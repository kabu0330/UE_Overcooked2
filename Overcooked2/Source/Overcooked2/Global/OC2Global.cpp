// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2Global.h"
#include "Overcooked2.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Global/OC2GameInstance.h"
#include "LevelContent/Cook/Ingredient.h"
#include "Global/GameMode/OC2GameMode.h"
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