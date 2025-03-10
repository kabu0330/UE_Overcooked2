// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/OC2GameInstance.h"
#include "Overcooked2.h"

#include "Global/Data/RecipeDataTable.h"
#include "Global/Data/IngredientDataTable.h"
#include "Global/Data/OrderDataTable.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UOC2GameInstance::UOC2GameInstance()
{

}

UOC2GameInstance::~UOC2GameInstance()
{
}

void UOC2GameInstance::CreateRoom()
{
	FString LobbyLevelName = LobbyLevel.GetLongPackageName();
	FString OpenLevelName = FString::Printf(TEXT(":%s%s"), *Port, *LobbyLevelName);

	if (false == LobbyLevelName.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), *OpenLevelName, true, TEXT("listen"));
	}
	else
	{
		UE_LOG(OVERCOOKED_LOG, Warning, TEXT("Lobby Level not setting"));
	}
}

void UOC2GameInstance::JoinRoom(FString IP, APlayerController* Controller)
{
	FString OpenLevelName = LobbyLevel.GetLongPackageName();
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s%s"), *IP, *Port, *OpenLevelName);

	if (Controller != nullptr)
	{
		Controller->ClientTravel(ConnectLevelName, ETravelType::TRAVEL_Absolute);
	}
}

void UOC2GameInstance::StartGame()
{
	FString OpenLevelPath = WorldLevel.GetLongPackageName();

	GetWorld()->ServerTravel(OpenLevelPath + TEXT("?listen"));
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

TArray<FPlateInitData> UOC2GameInstance::GetPlateMesh(TArray<FRecipe>& Recipes)
{
	static TArray<FPlateInitData> EmptyArray;
	TArray<FRecipeDataRow*> FindRecipes;
	
	TArray<FName> RowNames = RecipeDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		// 현재 행을 가져오기
		FRecipeDataRow* RecipeData = RecipeDataTable->FindRow<FRecipeDataRow>(RowName, nullptr);

		if (RecipeData->RequireIngredients.Num() == Recipes.Num()
			&& true == FindRecipe(RecipeData, Recipes))
		{
			TArray<FPlateInitData> Result;

			for (int i = 0; i < RecipeData->RequireIngredients.Num(); i++)
			{
				FPlateInitData PlateInitData;
				
				PlateInitData.StaticMesh = RecipeData->FoodMesh;
				PlateInitData.OffsetLocation = RecipeData->OffsetLocation;
				PlateInitData.OffsetRotation = RecipeData->OffsetRotation;
				
				Result.Add(PlateInitData);
			}

			return Result;
		}
	}

	return EmptyArray;
}

bool UOC2GameInstance::FindRecipe(const FRecipeDataRow* RecipeDataRow, TArray<FRecipe>& Recipes)
{
	if (nullptr == RecipeDataRow || 0 == RecipeDataRow->RequireIngredients.Num()|| 0 == Recipes.Num())
	{
		// UE_LOG(LogTemp, )
		return false;
	}

	for (int i = 0; i < RecipeDataRow->RequireIngredients.Num(); i++)
	{
		EIngredientType IngredientType = RecipeDataRow->RequireIngredients[i].IngredientType;
		EIngredientState IngredientState = RecipeDataRow->RequireIngredients[i].IngredientState;

		bool bFound = false;
		for (int j = 0; j < Recipes.Num(); j++)
		{
			if (Recipes[j].IngredientType == IngredientType && Recipes[j].IngredientState == IngredientState)
			{
				bFound = true;
				break;
			}
		}

		if (false == bFound)
		{
			return false;
		}
	}

	return true;
}

FString UOC2GameInstance::GetChefHeadName() const
{
	return ChefHeadName;
}

void UOC2GameInstance::SetChefHeadName(FString HeadName)
{
	ChefHeadName = HeadName;
}
