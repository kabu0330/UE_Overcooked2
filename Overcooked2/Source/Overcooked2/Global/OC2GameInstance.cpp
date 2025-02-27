// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2GameInstance.h"
#include "Overcooked2.h"

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
