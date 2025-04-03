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


	void PlayZoomOutAnimation();

	void PlayZoomInAnimation();


protected:
	virtual void NativeConstruct() override;


private:
	class UMaterialInstanceDynamic* TransitionMaterial = nullptr;
	FTimerHandle AnimationTimer;


};

