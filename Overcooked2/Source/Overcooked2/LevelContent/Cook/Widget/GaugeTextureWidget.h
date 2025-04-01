// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GaugeTextureWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UGaugeTextureWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProgressTimeRatio(float Ratio)
	{
		CookingTimeRatio = Ratio;
	}

	UFUNCTION(BlueprintCallable)
	float GetProgressTimeRatio() const
	{
		return CookingTimeRatio;
	}


protected:

private:
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	float CookingTimeRatio = 0.0f;
};
