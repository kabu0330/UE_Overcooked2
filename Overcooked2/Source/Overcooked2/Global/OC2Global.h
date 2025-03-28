// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Const.h"

#include "OC2Global.generated.h"

class UOC2GameInstance;
class AOC2Actor;
class AIngredient;
class APlate;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Global : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void GetAssetPackageName(UClass* Class, const FString& AssetName, FString& Path);

	UFUNCTION(BlueprintCallable)
	static TArray<FAssetData> GetAssetDataArray(UClass* Class);

	UFUNCTION(BlueprintCallable)
	static UOC2GameInstance* GetOC2GameInstance(const UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void StartServer(const UWorld* World, const FString& Port, const FString& LevelName);
	//static void StartServer_Implementation(const UWorld* World, const FString& Port, const FString& LevelName);

	UFUNCTION(BlueprintCallable)
	static void ConnectServer(const UWorld* World, APlayerController* Controller, const FString& IP, const FString& Port, const FString& LevelName);

	UFUNCTION(BlueprintCallable)
	static void TravelServer(UWorld* World, const FString& LevelName);

	UFUNCTION(BlueprintCallable)
	static void StartCookingStage(UWorld* World);
};
