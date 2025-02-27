// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


void UCookingWidget::OrderComplete()
{
    if (!Orders[CompleteOrderNum].BackgroundImage) return;

    GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);

}


void UCookingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    Orders.SetNum(3);

    Orders[0].BackgroundImage = OrderBackground_0; 
    Orders[1].BackgroundImage = OrderBackground_1;
    Orders[2].BackgroundImage = OrderBackground_2;

    float StartPosX = 20.0f;
    float offset = 10.0f;
    for (int i = 0; i < Orders.Num(); i++)
    {

        Orders[i].Position = { StartPosX +  offset * i, 0.0f};
        Orders[i].BackgroundImage->SetRenderTranslation(Orders[i].Position);
    }

}


void UCookingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateImagePosition(InDeltaTime);
}

void UCookingWidget::UpdateImageOpacity()
{

    if (Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity().A <= 0.0f)
    {
        Orders[CompleteOrderNum].BackgroundImage->SetVisibility(ESlateVisibility::Collapsed);

        for (int i = CompleteOrderNum; i < Orders.Num() - 1; i++)
        {
            //const TArray<UPanelSlot*> slots = CanvasPanel->GetSlots();
            //UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(slots[0]);
            //CPSlot->SetPosition(CPSlot->GetPosition() + FVector2D(100, 0));

            Orders[i + 1].BackgroundImage->SetRenderTranslation({ Orders[i].Position.X - Orders[i].BackgroundImage->GetDesiredSize().X, 0.0f});
        }

        for (int i = CompleteOrderNum; i < Orders.Num() - 1; i++)
        {

            Orders[i].BackgroundImage = Orders[i + 1].BackgroundImage;
        }


        GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
        return;
    }

    float CurOpacity = Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity().A;
    Orders[CompleteOrderNum].BackgroundImage->SetColorAndOpacity({ Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity().R, Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity().G, Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity().B, FMath::Clamp(CurOpacity - OpacityOffset, 0.0f, 1.0f) });
    


}

void UCookingWidget::UpdateImagePosition(float InDeltaTime)
{


}