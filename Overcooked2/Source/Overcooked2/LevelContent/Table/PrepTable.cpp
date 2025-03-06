// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/PrepTable.h"

APrepTable::APrepTable()
{

}

void APrepTable::BeginPlay()
{
	Super::BeginPlay();
}

void APrepTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACooking* APrepTable::Interact(AActor* ChefActor)
{
	return nullptr;
}
