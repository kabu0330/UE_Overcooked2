// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UProgressBarWidget();

	UFUNCTION(BlueprintCallable)
	void BindProgress(class UProgressBarComponent* ProgressBarComponent);

	UFUNCTION(BlueprintCallable)
	void UpdateWidget();

	class UProgressBarComponent* ProgressBarCompo = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgressRatio")
	float Ratio = 0.0f;
};
