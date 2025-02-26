// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "OC2CameraActor.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2CameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOC2CameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
