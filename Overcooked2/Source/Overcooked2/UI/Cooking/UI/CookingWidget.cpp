// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UCookingWidget::OrderComplete()
{
    if (!Orders[CompleteOrderNum].BackgroundImage) return;
    GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
}

void UCookingWidget::NewOrder()
{

    for (int i = 0; i < Orders.Num() - 1; i++)
    {
        if (Orders[i].bIsOrder == false)
        {
            NewOrderNum = i;
            break;
        }
    }
    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);



    Orders[NewOrderNum].BackgroundImage->SetVisibility(ESlateVisibility::Visible);
    Orders[NewOrderNum].BackgroundImage->SetRenderTransformAngle(-20.0f);
    const FLinearColor& curcolor = Orders[NewOrderNum].BackgroundImage->GetColorAndOpacity();

    Orders[NewOrderNum].BackgroundImage->SetColorAndOpacity({ curcolor.R, curcolor.G, curcolor.B, 1.0f});
    Orders[NewOrderNum].BackgroundImage->SetRenderTranslation({ ScreenSize.X * 1.5f, 0.0f });
    Orders[NewOrderNum].bIsOrder = true;

            Size++;

    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
}


void UCookingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Orders.SetNum(4);
    Size = Orders.Num();

    Orders[0].BackgroundImage = OrderBackground_0;
    Orders[1].BackgroundImage = OrderBackground_1;
    Orders[2].BackgroundImage = OrderBackground_2;
    
    // Test Setting
    Orders[0].bIsOrder = true;
    Orders[1].bIsOrder = true;
    //Orders[2].bIsOrder = true;

    for (int i = 0; i < Orders.Num() - 1; i++)
    {
        if (Orders[i].bIsOrder == false)
        {
           Orders[i].BackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

}


void UCookingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCookingWidget::MoveNewOrder()
{

    if (!Orders[NewOrderNum].BackgroundImage) return;

    FVector2D curpos = Orders[NewOrderNum].BackgroundImage->GetRenderTransform().Translation;
    float arrivepos = Orders[NewOrderNum - 1].BackgroundImage->GetRenderTransform().Translation.X + ImageOffset;
    float curangle = Orders[NewOrderNum].BackgroundImage->GetRenderTransform().Angle;

    if (curpos.X <= arrivepos && curangle >= 0.0f)
    {
        Orders[NewOrderNum].BackgroundImage->SetRenderScale({1.0f, 1.0f});
        Orders[NewOrderNum].BackgroundImage->SetRenderTranslation({ arrivepos,0.0f });



        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;
    }
    else
    {
        if (curpos.X > arrivepos)
        {
            Orders[NewOrderNum].BackgroundImage->SetRenderTranslation(curpos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));
        }
        if (curangle < 0.0f)
        {
            Orders[NewOrderNum].BackgroundImage->SetRenderTransformAngle(curangle + 0.5f);
        }
    }

    MoveTimeElapsed += 0.1f;
}



void UCookingWidget::UpdateImageOpacity()
{

    if (Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity().A <= 0.0f)
    {
        Orders[CompleteOrderNum].BackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
        UpdateImagePosition();

        GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
        return;
    }

    const FLinearColor& curcolor = Orders[CompleteOrderNum].BackgroundImage->GetColorAndOpacity();
    Orders[CompleteOrderNum].BackgroundImage->SetColorAndOpacity({ curcolor.R, curcolor.G, curcolor.B, FMath::Clamp(curcolor.A - OpacityOffset, 0.0f, 1.0f) });

}



void UCookingWidget::UpdateImagePosition()
{

    if (CompleteOrderNum + 1 == Orders.Num())
    {
        return;
    }

    Orders[Orders.Num() - 1] = Orders[CompleteOrderNum];

    // 앞으로 이동
    for (int i = CompleteOrderNum; i < Size - 2; i++)
    {
        if (Orders[i].bIsOrder == true)
        {
            const FVector2D& imagesize = Orders[i].BackgroundImage->GetDesiredSize();

            Orders[i] = Orders[i + 1];
            Orders[i].bIsOrder = true;
            const FWidgetTransform& trfm = Orders[i].BackgroundImage->GetRenderTransform();
            FinalPos = trfm.Translation.X - imagesize.X + ImageOffset;

            Orders[i].BackgroundImage->SetRenderTranslation({ trfm.Translation.X - imagesize.X + ImageOffset, 0.0f });
        }
    }



    if (Orders[CompleteOrderNum].BackgroundImage->GetRenderTransform().Translation.X <= FinalPos)
    {

        int LastNum = 0;
        for (int i = 1; i < Size - 1; i++)
        {
            if (Orders[i].bIsOrder == false)
            {
                LastNum = i - 1;

                //Orders[LastNum] = Orders[Orders.Num() - 1];
                Orders[LastNum].bIsOrder = false;

                break;
            }
        }

        Size--;

        return;
    }
}