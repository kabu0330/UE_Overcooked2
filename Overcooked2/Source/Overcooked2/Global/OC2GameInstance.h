// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Global/Data/IngredientDataTable.h"
#include "OC2GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2GameInstance : public UGameInstance
{
	GENERATED_BODY()

	//friend class UGlobla

public:
	UOC2GameInstance();
	~UOC2GameInstance();

	UFUNCTION(BlueprintCallable)
	EIngredientType GetIngredientType(const FString& RowName);

	UFUNCTION(BlueprintCallable)
	const TArray<FIngredientCookDataRow>& GetIngredientCookDataRows(const FString& RowName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UDataTable* IngredientDataTable = nullptr;
	
};
