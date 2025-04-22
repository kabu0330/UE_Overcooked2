// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LobbyMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ULobbyMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* StoryPanel = nullptr;

	void RemoveHoverButton();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;


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

	UFUNCTION()
	void HoverSubButton();

	class UCanvasPanel* CurPanel = nullptr;
	class UButton* CurButton = nullptr;
	class UButton* CurMainButton = nullptr;

	void UpdateMenuPosition();
	FTimerHandle MenuMoveTimerHandle;
	FSlateBrush CurButtonNomalBrush;

	float MenuMoveOffset = 10.0f;

	bool bIsCurPanel = false;
	bool bIsCurButton = false;

	template <typename T>
	TArray<T*> FindAllChildWidgets(UCanvasPanel* Panel);


	template <typename T>
	T* FindSiblingWidget(class UWidget* Widget);
};
