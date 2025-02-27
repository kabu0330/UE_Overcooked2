// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"

#include "Kismet/GameplayStatics.h"

EIngredientType UOC2GlobalData::GetIngredientType(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientType(RowName.ToString());
	}

	return EIngredientType::EIT_NONE;
}

const TArray<FIngredientCookDataRow>& UOC2GlobalData::GetIngredientCookDataRows(UWorld* World, const FName& RowName)
{
	static const TArray<FIngredientCookDataRow> EmptyArray;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientCookDataRows(RowName.ToString());
	}

	return EmptyArray;
}

UStaticMesh* UOC2GlobalData::GetIngredientStaticMesh(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientStaticMesh(RowName.ToString());
	}

	return nullptr;
}

FName UOC2GlobalData::GetIngredientDataTableRowName(UWorld* World, EIngredientType IngredientType)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientDataTableRowName(IngredientType);
	}

	return TEXT("");
}

const FIngredientDataRow* UOC2GlobalData::GetIngredientDataRow(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientDataRow(RowName);
	}

	return nullptr;
}
