// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2CameraActor.h"

// Sets default values
AOC2CameraActor::AOC2CameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOC2CameraActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOC2CameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOC2CameraActor::SetCameraScoreLocation()
{
	SetActorLocation({ -10250.0, -1820.0 , 255.0 });
	SetActorRotation({ 0, 0, 90 });
}