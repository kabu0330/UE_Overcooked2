// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/FireExtinguisher.h"

AFireExtinguisher::AFireExtinguisher()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_FIREEXTINGUISHER;
}

void AFireExtinguisher::BeginPlay()
{
	ACooking::BeginPlay();

}

void AFireExtinguisher::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

}
