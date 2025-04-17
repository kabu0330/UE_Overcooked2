// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyZoomInWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ULobbyZoomInWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void PlayZoomInAnimation();


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* TransitionImg = nullptr;


protected:
	virtual void NativeConstruct() override;


private:
	class UMaterialInstanceDynamic* TransitionMaterial = nullptr;
	FTimerHandle AnimationTimer;

	//TFunction<void()> AnimFinishFuction;

};
