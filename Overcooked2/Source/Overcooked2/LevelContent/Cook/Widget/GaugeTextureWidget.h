// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
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

	// Y축 양수 : 아래, 음수 : 위
	void SetPosition(FVector2D RelativeLocation)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar->Slot))
		{
			CanvasSlot->SetPosition(RelativeLocation);
		}
		if (UCanvasPanelSlot* BackSlot = Cast<UCanvasPanelSlot>(ProgressBarBack->Slot))
		{
			BackSlot->SetPosition(RelativeLocation);
		}
	}


protected:

private:
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	float CookingTimeRatio = 0.0f;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* ProgressBarBack = nullptr;
};
