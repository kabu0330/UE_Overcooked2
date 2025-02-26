// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Component/TimeEventComponent.h"
#include "Overcooked2.h"

UTimeEventComponent::UTimeEventComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTimeEventComponent::AddEvent(float Time, TFunction<void(float, float)> UpdateFunction, TFunction<void()> EndFunction, bool bLoop)
{
	FTimeEvent& NewEvent = Events.Emplace_GetRef();
	NewEvent.bLoop = bLoop;
	NewEvent.MaxTime = Time;
	NewEvent.CurTime = 0.0f;
	NewEvent.EndFunction = EndFunction;
	NewEvent.UpdateFunction = UpdateFunction;
}

void UTimeEventComponent::AddEndEvent(float Time, TFunction<void()> EndFunction, bool bLoop)
{
	AddEvent(Time, nullptr, EndFunction, bLoop);
}

void UTimeEventComponent::AddUpdateEvent(float Time, TFunction<void(float, float)> UpdateFunction, bool bLoop)
{
	AddEvent(Time, UpdateFunction, nullptr, bLoop);
}

void UTimeEventComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTimeEventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (true == Events.IsEmpty())
	{
		return;
	}

	for (size_t i = 0; i < Events.Num(); i++)
	{
		FTimeEvent& Event = Events[i];
		Event.CurTime += DeltaTime;
		if (nullptr != Event.UpdateFunction)
		{
			Event.UpdateFunction(DeltaTime, Event.CurTime);
		}
	}

	// 블루프린트로 
	for (size_t i = 0; i < Events.Num(); i++)
	{
		FTimeEvent& Event = Events[i];
		if (Event.MaxTime < Event.CurTime)
		{
			if (nullptr != Event.EndFunction)
			{
				Event.EndFunction();
			}

			if (false == Event.bLoop)
			{
				// 5번째 
				Events.RemoveAt(i);
				--i;
			}
			else
			{
				Event.CurTime = 0.0f;
			}
		}

	}
}

