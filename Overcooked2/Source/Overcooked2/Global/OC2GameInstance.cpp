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
