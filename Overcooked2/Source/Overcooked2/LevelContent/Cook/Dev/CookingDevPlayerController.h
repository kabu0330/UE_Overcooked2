// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CookingDevPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACookingDevPlayerController();
	~ACookingDevPlayerController();

	UFUNCTION(BlueprintCallable)
	void EnableUIMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;

private:

};
