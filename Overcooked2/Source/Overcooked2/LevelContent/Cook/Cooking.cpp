// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Cooking.h"
//#include <Global/Component/TimeEventActorComponent.h>

// Sets default values
ACooking::ACooking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DishMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DishMeshComponent"));
	RootComponent = DishMeshComponent;

	TimeEvent = CreateDefaultSubobject<UTimeEventActorComponent>(TEXT("TimeEvent"));
}

// Called when the game starts or when spawned
void ACooking::BeginPlay()
{
	Super::BeginPlay();
	
	if (DishMeshes.Num() > 0 && DishMeshes[0])
	{
		DishMeshComponent->SetStaticMesh(DishMeshes[0]);
	}
}

// Called every frame
void ACooking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACooking::SetMeshByIndex(int32 Index)
{
	if (true == DishMeshes.IsValidIndex(Index) && DishMeshes[Index])
	{
		DishMeshComponent->SetStaticMesh(DishMeshes[Index]);
	}
}

void ACooking::SetDebugMeshByIndex(int32 Index)
{
	if (true == DishMeshes.IsValidIndex(Index) && DishMeshes[Index])
	{
		// 테스트 로직, 2초 뒤에 해당 인덱스 메시로 바뀌어라
		//TimeEvent->AddEndEvent(2.0f, [this, Index]()
		//	{
		//		DishMeshComponent->SetStaticMesh(DishMeshes[Index]);
		//	});
	}

}

