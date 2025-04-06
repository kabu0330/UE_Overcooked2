// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingFinalScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookingFinalScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* PlayerCanvas = nullptr;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* PlayerImg_0 = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* PlayerImg_1 = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* PlayerImg_2 = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* PlayerImg_3 = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget")
	UTextureRenderTarget2D* RenderTarget0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget")
	UTextureRenderTarget2D* RenderTarget1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget")
	UTextureRenderTarget2D* RenderTarget2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget")
	UTextureRenderTarget2D* RenderTarget3;


	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void ShowCapturePlayers();
		
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:
	void ShowPlayers(int Index);

	int CurPlayerCount = 0;

	TArray<class UTextureRenderTarget2D*> RenderTargets;
	TArray<class UImage*> PlayerImgs;
	TArray<class UCaptureComponent2D*> CachedCaptureComponents;
	TArray<class AOC2Character*> PlayerCharacters;
	template <typename T>
	T* FindChildWidget(const FString& name, UCanvasPanel* canvas);
};
	