// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global/OC2Enum.h"
#include "IngredientDataTable.generated.h"

// 겹침 손질이 존재하는 데이터 속성
USTRUCT(BlueprintType)
struct FIngredientCookDataRow 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientState PrevIngredientState = EIngredientState::EIS_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientState IngredientState = EIngredientState::EIS_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* CookMesh = nullptr;
};

// 일반 재료 데이터 열
USTRUCT(BlueprintType)
struct FIngredientDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FIngredientDataRow() {}
	~FIngredientDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* BaseMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FIngredientCookDataRow> StateRows;
};

// 손질 가능한 요리 속성
USTRUCT(BlueprintType)
struct FCookableIngredient 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	EIngredientState IngredientState = EIngredientState::EIS_NONE;

};

// 재료 조합별 메쉬 데이터 속성
USTRUCT(BlueprintType)
struct FRecipeMeshDataRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FCookableIngredient> Array;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UStaticMesh* BaseMesh = nullptr;
};

// 레시피 데이터 열
USTRUCT(BlueprintType)
struct FRecipeDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FCookableIngredient> CookableIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	TArray<FRecipeMeshDataRow> CheckMesh;
};

// 스테이지에서 등장하는 레시피 데이터 열
USTRUCT(BlueprintType)
struct FStageCookingDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	//TArray<FName> StageRecipeNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	// 오징어
	// 오이
	// 김
	// 쌀
	
	// 상자

	TArray<EIngredientType> IngredientType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")

	TArray<FName> AllRecipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")

	TArray<int> SelectRecipe;

	// void Init();

};


/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UIngredientDataTable : public UObject
{
	GENERATED_BODY()
	
};
