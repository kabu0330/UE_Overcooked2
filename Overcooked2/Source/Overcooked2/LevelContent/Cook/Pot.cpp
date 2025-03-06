// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Pot.h"

APot::APot()
{
	SoupSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SoupSkeletalMeshComponent"));
	SoupSkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
}

void APot::Boiling(ACooking* Rice)
{
}

void APot::CookRice()
{
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();
}

void APot::Tick(float DeltaTime)
{
	ACooking::BeginPlay();
}
