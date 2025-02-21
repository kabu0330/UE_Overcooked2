// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GlobalDevPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AGlobalDevPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
