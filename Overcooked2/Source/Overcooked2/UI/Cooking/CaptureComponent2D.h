// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CaptureComponent2D.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCaptureComponent2D : public USceneCaptureComponent2D
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	UCaptureComponent2D();

	void ShowOnlyActor(class AActor* Character);

protected:
	void BeginPlay();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
private:

};
