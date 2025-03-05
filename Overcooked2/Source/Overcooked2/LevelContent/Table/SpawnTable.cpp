// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/SpawnTable.h"
#include <Global/Data/IngredientDataTable.h>
#include "Kismet/GameplayStatics.h"

ASpawnTable::ASpawnTable()
{
}

void ASpawnTable::Init(FName Name)
{
	IngredientName = Name;
}

AIngredient* ASpawnTable::SpawnIngredient(AActor* ChefActor)
{

	FActorSpawnParameters SpawnParameters; // 적절한 오버로딩 함수 호출을 위해(회전값 추가), FActorSpawnParameters 사용
	FVector Location = FVector();
	if (nullptr != ChefActor)
	{
		Location = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	}
	FRotator Rotator = FRotator::ZeroRotator;

	// 1. 재료를 월드에 스폰한다.
	// Transform은 있지만 메시도 없는 빈 껍데기 상태
	AIngredient* NewIngredient = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), Location, Rotator, SpawnParameters);

	// 액터에 부착
	if (nullptr != ChefActor)
	{
		//NewIngredient->Interact(ChefActor);
	}

	if (nullptr == NewIngredient)
	{
		UE_LOG(LogTemp, Warning, TEXT("액터 스폰에 실패했습니다."));
		return nullptr;
	}
	
	// 2. 메시를 찾아서
//	NewIngredient->Init(IngredientName);

	return NewIngredient;
}

ACooking* ASpawnTable::Interact(AActor* ChefActor)
{
	if (false == bOccupied)
	{
		return SpawnIngredient(ChefActor);
	}

	return nullptr;
}
