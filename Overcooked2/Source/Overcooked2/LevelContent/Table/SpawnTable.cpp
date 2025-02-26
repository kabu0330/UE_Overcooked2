// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/SpawnTable.h"
#include <Global/Data/IngredientDataTable.h>
#include "Kismet/GameplayStatics.h"

ASpawnTable::ASpawnTable()
{
}


AIngredient* ASpawnTable::SpawnIngredient(FName IngredientName)
{
	//UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable'_C"));

	//if (nullptr == DataTable)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IngredientDataTable을 찾지 못해 재료 스폰에 실패했습니다."));
	//	return nullptr;
	//}

	//FName RowName = *StaticEnum<EIngredientType>()->GetDisplayNameTextByValue(static_cast<int64>(Ingredient)).ToString();
	//FName RowName = *StaticEnum<EIngredientType>()->GetDisplayNameTextByValue(static_cast<int64>(Ingredient)).ToString();
	//FIngredientDataRow* IngredientData = DataTable->FindRow<FIngredientDataRow>(RowName, TEXT(""));

	//if (nullptr == IngredientData)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IngredinetDataTable에서 재료 타입과 일치하는 데이터를 추출에 실패했습니다."));
	//	return nullptr;
	//}

	FActorSpawnParameters SpawnParameters; // 적절한 오버로딩 함수 호출을 위해(회전값 추가), FActorSpawnParameters 사용
	FVector Location = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f); 
	FRotator Rotator = FRotator(90.0f, 90.0f, 90.0f);

	// 1. 재료를 월드에 스폰한다.
	// Transform은 있지만 메시도 없는 빈 껍데기 상태
	AIngredient* NewIngredient = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), Location, Rotator, SpawnParameters);

	if (nullptr == NewIngredient)
	{
		UE_LOG(LogTemp, Warning, TEXT("액터 스폰에 실패했습니다."));
		return nullptr;
	}
	
	// 2. 메시를 찾아서
	NewIngredient->Init(IngredientName);

	return NewIngredient;
}
