// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeEventComponent.generated.h"

USTRUCT()
struct OVERCOOKED2_API FTimeEvent
{
	GENERATED_BODY()

public:
	bool bLoop = false;
	TFunction<void(float, float)> UpdateFunction;
	TFunction<void()> EndFunction;
	float CurTime = 0.0f;
	float MaxTime = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API UTimeEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTimeEventComponent();

	void AddEvent(float Time, TFunction<void(float, float)> UpdateFunction, TFunction<void()> EndFunction, bool bLoop = false);
	void AddEndEvent(float Time, TFunction<void()> EndFunction, bool bLoop = false);
	void AddUpdateEvent(float Time, TFunction<void(float, float)> UpdateFunction, bool bLoop = false);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	TArray<FTimeEvent> Events;

};
