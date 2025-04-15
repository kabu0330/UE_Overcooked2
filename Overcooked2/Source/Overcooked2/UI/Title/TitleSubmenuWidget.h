// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Title/TitleWidget.h"
#include "TitleSubmenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UTitleSubmenuWidget : public UTitleWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* IP_Panel = nullptr;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void MoveBanner();

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void PlayHoverSound();


	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void PlayPressSound();


protected:
	virtual void NativeOnInitialized() override;

private:
	// TimerHandle
	FTimerHandle MoveTimerHandle;
	float MoveYOffset = 0.0f;

	void UpdateBannerPosition();


};
