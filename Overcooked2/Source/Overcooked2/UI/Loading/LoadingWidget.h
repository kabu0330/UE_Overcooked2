// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetProgress(float Value);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UProgressBar* LoadingProgressBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* TransitionImg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* ConnectingImage = nullptr;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* ConnectingCanvas = nullptr;

	void PlayZoomOutAnimation();

	void PlayZoomInAnimation();
	void PlayLoadingAnimation(TFunction<void()> Func);


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);


private:
	class UMaterialInstanceDynamic* TransitionMaterial = nullptr;
	class UMaterialInstanceDynamic* ConnectingMaterial = nullptr;

	FTimerHandle AnimationTimer;
	FTimerHandle LoadingAnimationTimer;

	bool bIsConnecting = false;
	float ProgressTime = 0.0f;
	float AnimationTotalIndex = 0.45f;
	float CurIndex = 0.0f;
	float CurTime = 0.0f;

	TFunction<void()> Function;
};

