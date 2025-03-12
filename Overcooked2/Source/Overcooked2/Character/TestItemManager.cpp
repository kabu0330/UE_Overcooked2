// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestItemManager.h"
#include "Character/OC2CharacterTestObject.h"
#include "Global/OC2Global.h"

// Sets default values for this component's properties
UTestItemManager::UTestItemManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTestItemManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AIngredient* UTestItemManager::CreateIngredient(EIngredientType Type)
{
	FTransform Trans;
	AIngredient* NewIngredient = GetWorld()->SpawnActorDeferred<AIngredient>(AIngredient::StaticClass(), Trans);

	// 2. 메시를 찾아서
	NewIngredient->SetType(Type);

	Trans.SetLocation(FVector(100.0f, 100.0f, 0.0f));
	NewIngredient->FinishSpawning(Trans);

	return NewIngredient;
}


// Called every frame
void UTestItemManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

