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

	/**
	 * 재료의 유형을 가져옵니다.
	 * 
	 * @param RowName 데이터 테이블에서 찾을 행의 이름입니다.
	 * @return 해당 재료의 유형(EIngredientType)입니다.
	 */
	UFUNCTION(BlueprintCallable)
	EIngredientType GetIngredientType(const FString& RowName);
	/**
	 * 특정 재료의 조리 데이터를 가져옵니다.
	 * 
	 * @param RowName 데이터 테이블에서 찾을 행의 이름입니다.
	 * @return 해당 재료의 조리 데이터 배열(TArray<FIngredientCookDataRow>)입니다.
	 */
	UFUNCTION(BlueprintCallable)
	const TArray<FIngredientCookDataRow>& GetIngredientCookDataRows(const FString& RowName);
	/**
	 * 특정 재료의 스태틱 메시를 가져옵니다.
	 * 
	 * @param RowName 데이터 테이블에서 찾을 행의 이름입니다.
	 * @return 해당 재료의 스태틱 메시(UStaticMesh*)입니다.
	 */
	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetIngredientStaticMesh(const FString& RowName);
	/**
	 * @brief 주어진 재료 타입(EIngredientType)에 해당하는 데이터 테이블 행 이름을 반환합니다.
	 *
	 * @param IngredientType 조회할 재료의 타입입니다.
	 * @return FName 해당 재료의 데이터 테이블 행 이름을 반환합니다. 유효하지 않은 타입일 경우 빈 FName("")을 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ingredients")
	FName GetIngredientDataTableRowName(EIngredientType IngredientType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UDataTable* IngredientDataTable = nullptr;
	
};
