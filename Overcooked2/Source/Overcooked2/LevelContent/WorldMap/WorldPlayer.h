// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorldPlayer.generated.h"

UCLASS()
class OVERCOOKED2_API AWorldPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AWorldPlayer();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InputMove(const FVector2D& _Vec);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
