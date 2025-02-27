// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "CookingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACookingPlayerController();
	~ACookingPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|PlayerController", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;
};
