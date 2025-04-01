// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProgressBarComponent.generated.h"


//DECLARE_MULTICAST_DELEGATE(FProgressDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API UProgressBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProgressBarComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeProgress(float DeltaTime);
	void CountProgress(float DeltaTime);
	void TimerUpdate(float DeltaTime);

	bool IsProgressDone()
	{
		return bProgressDone;
	}

	float Timer = 0.0f;
	float CurTime = 0.0f;
	bool bProgressDone = false;
		
};
