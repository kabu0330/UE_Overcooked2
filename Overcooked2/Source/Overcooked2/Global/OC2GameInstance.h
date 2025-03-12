// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"

#include "Global/Data/IngredientDataTable.h"
#include "Global/Data/OrderDataTable.h"

#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"

#include "OC2GameInstance.generated.h"

class AIngredient;
struct FRecipeDataRow;

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

public: /** 네트워크 관련 메소드 */
	void CreateRoom();
	void JoinRoom(FString IP, APlayerController* Controller);
	void StartGame();

public:
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
	UFUNCTION(BlueprintCallable, Category = "Global|Data")
	FName GetIngredientDataTableRowName(EIngredientType IngredientType);
	/**
	 * 특정 재료의 데이터 테이블 행 데이터를 참조 하여 가져옵니다.
	 *
	 * @param World 게임 월드 객체입니다.
	 * @return FIngredientDataRow 해당 재료의 데이터 테이블 행 데이터입니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Global|Data")
	const FIngredientDataRow& GetIngredientDataRow(const FName& RowName);
	/**
	 * 특정 재료의 데이터 테이블 행 데이터를 타입을 사용하여
	 *
	 * @param World 게임 월드 객체입니다.
	 * @return FIngredientDataRow 해당 재료의 데이터 테이블 행 데이터입니다.
	 */
	const FIngredientDataRow& GetIngredientDataRow(EIngredientType IngredientType);


	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SpawnIngredientActor(EIngredientType IngredientType);

	TArray<FPlateInitData> GetPlateMesh(TArray<FRecipe>& Recipes);
	bool FindRecipe(const FRecipeDataRow* RecipeDataRow, TArray<FRecipe>& Recipes);

public:
	FString GetChefHeadName() const;
	void SetChefHeadName(FString HeadName);

public:
	UPROPERTY(EditAnywhere, Category = "Global|Level")
	TSoftObjectPtr<UWorld> TitleLevel;

	UPROPERTY(EditAnywhere, Category = "Global|Level")
	TSoftObjectPtr<UWorld> LobbyLevel;

	UPROPERTY(EditAnywhere, Category = "Global|Level")
	TSoftObjectPtr<UWorld> WorldLevel;

	UPROPERTY(EditAnywhere, Category = "Global|Level")
	TSoftObjectPtr<UWorld> CookingLevel;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UDataTable* IngredientDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UDataTable* OrderDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UDataTable* RecipeDataTable = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Global|Data")
	FString Port = TEXT("3000");

private:
	UPROPERTY(EditAnywhere, Category = "Global|Character")
	FString ChefHeadName = "";
};
