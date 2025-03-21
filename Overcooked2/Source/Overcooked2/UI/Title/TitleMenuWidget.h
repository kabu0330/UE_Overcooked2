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
	class UCanvasPanel* StoryPanel = nullptr;


protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* StoryButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* ArcadeButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* BattleButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* ChefButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* OptionButton = nullptr;

private:
	TArray<class UButton*> Buttons;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void HoverButton();

	class UCanvasPanel* CurPanel = nullptr;


	void UpdateMenuPosition();
	FTimerHandle MenuMoveTimerHandle;
	float MenuMoveOffset = 10.0f;

	template <typename T>
	T* FindChildWidget(const FString& name, class UCanvasPanel* canvas);

	template <typename T>
	T* FindSiblingWidget(class UWidget* Widget);

};
