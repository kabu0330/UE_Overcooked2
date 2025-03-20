// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingScoreWidget.generated.h"

/**
 *
 */
UCLASS()
class OVERCOOKED2_API UCookingScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void PlayCoinAnimation();

protected:
	virtual void NativeConstruct() override;
private:
	class UMaterialInstanceDynamic* CoinMaterial;

	FTimerHandle AnimationTimer;
};
