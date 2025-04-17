// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMapUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UWorldMapUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable, Category = "OC2UI")
	//void PlayZoomInAnimation();

	void PlayZoomInAnimation();
	void PlayZoomOutAnimation();

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void SettingDebugMessage(FString Text);


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* TransitionImg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UTextBlock* TestTxt = nullptr;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


private:
	class UMaterialInstanceDynamic* TransitionMaterial = nullptr;
	FTimerHandle AnimationTimer;
	FString PreviousText = "";
	//TFunction<void()> AnimFinishFuction;



};
