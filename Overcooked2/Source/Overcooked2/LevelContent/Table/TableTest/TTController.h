// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "TTController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ATTController : public APlayerController
{
	GENERATED_BODY()

	void SetupInputComponent() override;
	
public:

	/*UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupInputComponentEvent();*/

	/*UFUNCTION(BlueprintCallable)
	void AddMappingContext(UInputMappingContext* MappingContext);*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* MappingContext = nullptr;
};
