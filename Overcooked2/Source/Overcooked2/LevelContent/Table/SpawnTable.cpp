// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/SpawnTable.h"
#include <Global/Data/IngredientDataTable.h>

ASpawnTable::ASpawnTable()
{
}

void ASpawnTable::Init(EIngredientType)
{
}

AIngredient* ASpawnTable::SpawnIngredient(EIngredientType Ingredient)
{
	// LoadObject : 특정 경로에 있는 UObject 기반 에셋 로드 함수
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable'_C"));

	if (nullptr == DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("IngredientDataTable을 찾지 못해 재료 스폰에 실패했습니다."));
		return nullptr;
	}

	FName RowName = *UEnum::GetValueAsString(Ingredient);
	FIngredientDataRow* IngredientData = DataTable->FindRow<FIngredientDataRow>(RowName, TEXT(""));

	if (nullptr == IngredientData)
	{
		UE_LOG(LogTemp, Warning, TEXT("IngredinetDataTable에서 재료 타입과 일치하는 데이터를 추출에 실패했습니다."));
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	FVector Location = GetActorLocation();
	// 적절한 오버로딩 함수 호출을 위해(회전값), FActorSpawnParameters 사용
	AIngredient* NewIngredient = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), Location, FRotator::ZeroRotator, SpawnParameters); 

	if (nullptr == NewIngredient)
	{
		UE_LOG(LogTemp, Warning, TEXT("액터 스폰에 실패했습니다."));
		return nullptr;
	}
	
	NewIngredient->Init(RowName);

	return NewIngredient;
}
