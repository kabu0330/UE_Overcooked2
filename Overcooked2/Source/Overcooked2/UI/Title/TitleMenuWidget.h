// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Title/TitleWidget.h"
#include "TitleMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UTitleMenuWidget : public UTitleWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* StoryPanel;


protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* StoryButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* ArcadeButton;

private:
	TArray<class UButton*> Buttons;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void OnButtonHovered();

	template <typename T>
	T* FindChildWidget(const FString& name, class UCanvasPanel* canvas);

	template <typename T>
	T* FindSiblingWidget(class UWidget* Widget);

};
