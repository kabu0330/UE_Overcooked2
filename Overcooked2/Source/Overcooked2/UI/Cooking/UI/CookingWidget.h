// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingWidget.generated.h"

/**
 *
 */

UCLASS()
class OVERCOOKED2_API UCookingWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    //Test
    UFUNCTION(BlueprintCallable, Category = "OC2UI")
    void SetCompleteOrderNum(int num)
    {
        CompleteOrderNum = num;
    }


    UFUNCTION(BlueprintCallable, Category = "OC2UI")
    void OrderComplete();

    UFUNCTION(BlueprintCallable, Category = "OC2UI")
    void CreateNewOrder(struct FOrder& order);


    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_0;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_3;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_4;

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

private:
    int CompleteOrderNum = 0;
    int NewOrderNum = 0;

    TArray <class UCanvasPanel*> Orders;


    FTimerHandle OpacityTimerHandle;
    float OpacityOffset = 0.05f;

    FTimerHandle MoveTimerHandle;
    FVector2D TargetOffset = FVector2D(50.0f, 0.0f);
    float MoveTimeElapsed = 10.0f;

    FTimerHandle IngredientTimerHandle;
    FVector2D IngredientTargetOffset = FVector2D(0.0f, 10.0f);
    float IngredientTimeElapsed = 10.0f;
    float IngredientArrivePos = 100.f;

    float ImageOffset = 20.0f;
    float FinalPos = 0.0f;
    float ArrivePos = 0.0f;
    int CurOrderCount = 0;

    float ImageSize = 180.0f;

    void UpdateImageOpacity();
    void UpdateImagePosition();
    void UpdateIngredientImagePosition();

    void SettingIngredientImages(FOrder& order);
    FVector2D IShortSize = { 56.0f, 60.f };
    FVector2D ILongSize = { 56.0f, 90.f };

    void MoveNewOrder();


    template <typename T>
    T* FindChildWidget(const FString& name, UCanvasPanel* canvas);
    class UImage* FindChildImage(const FString& name, class UCanvasPanel* canvasepanel);
    class UCanvasPanel* FindChildPanel(const FString& name, UCanvasPanel* canvase);

};
