// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2GameInstance.h"
#include "Overcooked2.h"

#include "Global/Data/RecipeDataTable.h"
#include "Global/Data/IngredientDataTable.h"
#include "Global/Data/OrderDataTable.h"

UOC2GameInstance::UOC2GameInstance()
{

}

UOC2GameInstance::~UOC2GameInstance()
{
}

EIngredientType UOC2GameInstance::GetIngredientType(const FString& RowName)
{
	if (nullptr != IngredientDataTable)
	{

		FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(*RowName, nullptr);

		if (nullptr != IngredientData)
		{
			return IngredientData->IngredientType;
		}
	}

	return EIngredientType::EIT_NONE;
}

const TArray<FIngredientCookDataRow>& UOC2GameInstance::GetIngredientCookDataRows(const FString& RowName)
{
	static const TArray<FIngredientCookDataRow> EmptyArray;

	if (nullptr != IngredientDataTable)
	{
		FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(*RowName, nullptr);

		if (nullptr != IngredientData)
		{
			return IngredientData->StateRows;
		}
	}

	return EmptyArray;
}

UStaticMesh* UOC2GameInstance::GetIngredientStaticMesh(const FString& RowName)
{
	if (nullptr != IngredientDataTable)
	{
		FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(*RowName, nullptr);

		if (nullptr != IngredientData)
		{
			return IngredientData->BaseMesh;
		}
	}

	return nullptr;
}

FName UOC2GameInstance::GetIngredientDataTableRowName(EIngredientType IngredientType)
{
	switch (IngredientType)
	{
	case EIngredientType::EIT_FISH:
		return TEXT("Fish");
	case EIngredientType::EIT_CABBAGE:
		return TEXT("Cabbage");
	case EIngredientType::EIT_PRAWN:
		return TEXT("Prawn");
	case EIngredientType::EIT_TOMATO:
		return TEXT("Tomato");
	case EIngredientType::EIT_LAVER:
		return TEXT("Laver");
	case EIngredientType::EIT_RICE:
		return TEXT("Rice");
	case EIngredientType::EIT_CUCUMBER:
		return TEXT("Cucumber");
	default:
		return TEXT("");
	}
}

const FIngredientDataRow& UOC2GameInstance::GetIngredientDataRow(const FName& RowName)
{
	static FIngredientDataRow EmptyData;

	if (nullptr != IngredientDataTable)
	{
		FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(RowName, nullptr);

		if (nullptr != IngredientData)
		{
			return *IngredientData;
		}
	}

	return EmptyData;
}

const FIngredientDataRow& UOC2GameInstance::GetIngredientDataRow(EIngredientType FindIngredientType)
{
	static FIngredientDataRow EmptyData;

	// 데이터 테이블의 모든 행의 이름을 가져오기
	TArray<FName> RowNames = IngredientDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		// 현재 행을 가져오기
		FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(RowName, nullptr);

		if (nullptr != IngredientData && IngredientData->IngredientType == FindIngredientType)
		{
			return *IngredientData; // 원하는 값이 있는 행을 찾으면 반환
		}
	}

	return EmptyData;
}

const UStaticMesh* UOC2GameInstance::GetPlateMesh(TArray<FRecipe>& Recipes)
{
	TArray<FCookableIngredient> FindRecipes;
	
	TArray<FName> RowNames = RecipeDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		// 현재 행을 가져오기
		FRecipeDataRow* RecipeData = RecipeDataTable->FindRow<FRecipeDataRow>(RowName, nullptr);

		if (RecipeData->RequireIngredients.Num() == Recipes.Num())
		{
			for (int i = 0; i < RecipeData->RequireIngredients.Num(); i++)
			{
				FCookableIngredient Data = RecipeData->RequireIngredients[i];

				FindRecipes.Push(Data);
			}
		}
	}

	return nullptr;
}
