// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameFramework/OC2Actor.h"

AOC2Actor::AOC2Actor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AOC2Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOC2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOC2Actor::Interact(AActor* ChefActor)
{
}

