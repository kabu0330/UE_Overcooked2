// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Interface/Interactable.h"
#include "OC2Actor.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2Actor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AOC2Actor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* ChefActor) override;

public:	

};
