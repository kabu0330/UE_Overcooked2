// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Cooking.h"
#include <Global/Component/TimeEventComponent.h>

// Sets default values
ACooking::ACooking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeEvent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEvent"));

	//// 추상클래스여서 UPrimitiveComponent로 직접 생성 불가.
	//PrimitiveComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimitiveComponent"));

	if (nullptr != StaticMeshComponent)
	{
		StaticMeshComponent->SetMobility(EComponentMobility::Movable);
		StaticMeshComponent->SetSimulatePhysics(true);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	}
	//Cast<UStaticMeshComponent>(PrimitiveComponent)->SetSimulatePhysics(true);
	

	bReplicates = true;
}

// Called when the game starts or when spawned
void ACooking::BeginPlay()
{
	AOC2Actor::BeginPlay();
	
}

// Called every frame
void ACooking::Tick(float DeltaTime)
{
	AOC2Actor::Tick(DeltaTime);

}

