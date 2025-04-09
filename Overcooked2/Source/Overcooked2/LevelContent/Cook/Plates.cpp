// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Plates.h"
#include <Net/UnrealNetwork.h>

APlates::APlates()
{
}

void APlates::Add_Implementation(APlate* Plate)
{
	bIsCombinationSuccessful = false;
	if (true == Plate->IsDirtyPlate())
	{
		Plates.Add(Plate);

		SetRelativePosition();

		bIsCombinationSuccessful = true;
	}
}

void APlates::BeginPlay()
{
	ACooking::BeginPlay();
}

void APlates::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);
}

void APlates::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlates, Plates);
	DOREPLIFETIME(APlates, bIsCombinationSuccessful);
}

void APlates::SetRelativePosition()
{

}
