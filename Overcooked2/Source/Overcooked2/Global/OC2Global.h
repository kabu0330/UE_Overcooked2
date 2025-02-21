// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OC2Global.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Global : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void GetAssetPackageName(UClass* Class, const FString& AssetName, FString& Path);

	UFUNCTION(BlueprintCallable)
	static TArray<FAssetData> GetAssetDataArray(UClass* Class);
	
};
