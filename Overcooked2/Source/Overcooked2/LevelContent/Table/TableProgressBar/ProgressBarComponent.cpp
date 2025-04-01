// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/TableProgressBar/ProgressBarComponent.h"

// Sets default values for this component's properties
UProgressBarComponent::UProgressBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProgressBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProgressBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProgressBarComponent::InitializeProgress(float DeltaTime)
{
	bProgressDone = false;
	CurRatio = 0.0f;
	ProgressDelegate.Broadcast();
}

void UProgressBarComponent::CountProgress(float DeltaTime)
{
	CurRatio = (DeltaTime / 0.4f) * 0.2f;

	if (CurRatio > 1.0f)
	{
		bProgressDone = true;
	}
}
