// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AWorldPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddInputMappingContext(UInputMappingContext* _Context);

private:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category="Controller", meta=(AllowPrivateAccess=true))
	UInputMappingContext* MappingContext = nullptr;
};
