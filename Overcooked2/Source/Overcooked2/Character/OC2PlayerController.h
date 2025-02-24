// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "OC2PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AOC2PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction = nullptr;
};
