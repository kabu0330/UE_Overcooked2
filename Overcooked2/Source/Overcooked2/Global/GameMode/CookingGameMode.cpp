// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameMode/CookingGameMode.h"

ACookingGameMode::ACookingGameMode()
{
	OrderManager = CreateDefaultSubobject<UOrderManageComponent>(TEXT("OrderManager"));
}

void ACookingGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
