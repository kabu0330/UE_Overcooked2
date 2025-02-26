// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2GameInstance.h"
#include "Overcooked2.h"

UOC2GameInstance::UOC2GameInstance()
{
	//{
	//	FString DataPath = TEXT("/Script/Engine.DataTable'/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable'");
	//	ConstructorHelpers::FObjectFinder<UDataTable> FinderDataTables(*DataPath);

	//	if (true == FinderDataTables.Succeeded())
	//	{
	//		IngredientDataTable = FinderDataTables.Object;
	//	}

	//	static const FString RowName = TEXT("Fish");
	//	FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(*RowName, nullptr);

	//	if (IngredientData)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("IngredientType: %d"), (int32)IngredientData->IngredientType);

	//		for (const auto& CookData : IngredientData->StateRows)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("PrevState: %d, State: %d"),
	//				(int32)CookData.PrevIngredientState,
	//				(int32)CookData.IngredientState);
	//		}
	//	}

	//}
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
	//FString DataPath = TEXT("/Script/Engine.DataTable'/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable'");
	//ConstructorHelpers::FObjectFinder<UDataTable> FinderDataTables(*DataPath);

	//if (true == FinderDataTables.Succeeded())
	//{
	//	IngredientDataTable = FinderDataTables.Object;
	//}

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
