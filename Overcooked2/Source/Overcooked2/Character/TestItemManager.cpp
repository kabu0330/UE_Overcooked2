// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestItemManager.h"
#include "Character/OC2CharacterTestObject.h"

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

AActor* UTestItemManager::CreateItem(EIngredientType Type)
{
	FTransform Trans;
	AOC2CharacterTestObject* NewIngredient = GetWorld()->SpawnActorDeferred<AOC2CharacterTestObject>(AOC2CharacterTestObject::StaticClass(), Trans);

	// 2. 메시를 찾아서
	NewIngredient->SetIngredient(Type);

	Trans.SetLocation(FVector(100.0f, 100.0f, 0.0f));
	NewIngredient->FinishSpawning(Trans);

	return NewIngredient;

	//ItemActor->SetItemDataKey(_ItemName);
	//// 디퍼드를 하면 왠만하면 이걸 호출해주는것이 좋습니다.
	//// 생성과정을 완전히 끝내고 월드에 제대로 편입시키는 함수입니다.
	//ItemActor->FinishSpawning(Trans);

	// 안되는 방법들
	// NetMultiCast 함수를 써서 
	// ItemActor->DataSetting();


	//return ItemActor;
}


// Called every frame
void UTestItemManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

