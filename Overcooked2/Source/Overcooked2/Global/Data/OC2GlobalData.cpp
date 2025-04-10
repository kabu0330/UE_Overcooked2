// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2Global.h"
#include "Global/OC2GameInstance.h"

#include "LevelContent/Cook/Ingredient.h"

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

const FIngredientDataRow& UOC2GlobalData::GetIngredientDataRow(UWorld* World, const FName& RowName)
{
	static FIngredientDataRow EmptyData;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientDataRow(RowName);
	}	

	return EmptyData;
}

const FIngredientDataRow& UOC2GlobalData::GetIngredientDataRow(UWorld* World, EIngredientType IngredientType)
{
	static FIngredientDataRow EmptyData;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetIngredientDataRow(IngredientType);
	}

	return EmptyData;
}

FPlateInitData UOC2GlobalData::GetPlateMesh(UWorld* World, TArray<FRecipe>& Recipes)
{
	static FPlateInitData EmptyArray;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetPlateMesh(Recipes);
	}

	return EmptyArray;
}

FPlateInitData UOC2GlobalData::GetPlateMesh(UWorld* World, const TArray<class AIngredient*>& Ingredients)
{
	static FPlateInitData EmptyArray;

	TArray<FRecipe> Recipes;

	for (int i = 0; i < Ingredients.Num(); i++)
	{
		FRecipe Recipe;

		Recipe.IngredientState = Ingredients[i]->GetCurIngredientState();
		Recipe.IngredientType = Ingredients[i]->GetIngredientType();

		Recipes.Add(Recipe);
	}

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetPlateMesh(Recipes);
	}

	return EmptyArray;
}

FOrder UOC2GlobalData::GetOrderByStageAndIndex(UWorld* World, EOC2Stage CurStage, int Index)
{
	static FOrder Empty;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetOrderByStageAndIndex(CurStage, Index);
	}

	return Empty;
}

const FResourceTextureDataRow& UOC2GlobalData::GetResourceTextureDataRow(UWorld* World, const FName& RowName)
{
	// TODO: 여기에 return 문을 삽입합니다.
	static FResourceTextureDataRow EmptyData;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceTextureDataRow(RowName);
	}

	return EmptyData;
}

UTexture* UOC2GlobalData::GetResourceTexture(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceTextureDataRow(RowName).TextureRes;
	}

	return nullptr;
}

const FResourceNiagaraDataRow& UOC2GlobalData::GetResourceNiagaraDataRow(UWorld* World, const FName& RowName)
{
	static FResourceNiagaraDataRow EmptyData;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceNiagaraDataRow(RowName);
	}

	return EmptyData;
}

UNiagaraSystem* UOC2GlobalData::GetResourceNiagaraSystem(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceNiagaraDataRow(RowName).NiagaraSystem;
	}

	return nullptr;
}

FVector UOC2GlobalData::GetResourceNiagaraLocation(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceNiagaraDataRow(RowName).Location;
	}

	return FVector::ZeroVector;
}

FRotator UOC2GlobalData::GetResourceNiagaraRotator(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceNiagaraDataRow(RowName).Rotation;
	}

	return FRotator::ZeroRotator;
}

FVector UOC2GlobalData::GetResourceNiagaraScale(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceNiagaraDataRow(RowName).Scale;
	}

	return FVector::ZeroVector;
}

UStaticMesh* UOC2GlobalData::GetResourceStaticMesh(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceStaticMeshDataRow(RowName).StaticMesh;
	}

	return nullptr;
}

UMaterial* UOC2GlobalData::GetResourceMaterial(UWorld* World, const FName& RowName)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetResourceMaterialDataRow(RowName).Material;
	}

	return nullptr;
}

UTexture2D* UOC2GlobalData::GetChefTexture(UWorld* World)
{
	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetChefTexture();
	}

	return nullptr;
}

FOrder UOC2GlobalData::GetOrderByIngredients(UWorld* World, ACooking* Cooking)
{
	FOrder Result;

	UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(World);

	if (nullptr != GameInstance)
	{
		return GameInstance->GetOrderByRecipes(Cooking);
	}

	return Result;
}
