// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Map/CityCar.h"

// Sets default values
ACityCar::ACityCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	UniqueShakeOffset = FMath::RandRange(-0.5f, 0.5f);
}

// Called when the game starts or when spawned
void ACityCar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACityCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Time = GetWorld()->GetTimeSeconds();

	float ShakeX = FMath::PerlinNoise1D((Time + UniqueShakeOffset) * 3.0f) * 0.3f;
	float ShakeY = FMath::PerlinNoise1D((Time + UniqueShakeOffset) * 2.0f) * 0.3f;

	FVector EngineShake = FVector(ShakeX, ShakeY, 0.0f);
	SetActorRelativeLocation(GetActorLocation() + EngineShake);
}

