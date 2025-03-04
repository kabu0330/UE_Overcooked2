// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UCookingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Orders.SetNum(5);

    Orders[0].BackgroundImage = OrderBackground_0;
    Orders[1].BackgroundImage = OrderBackground_1;
    Orders[2].BackgroundImage = OrderBackground_2;
    Orders[3].BackgroundImage = OrderBackground_3;
    Orders[4].BackgroundImage = OrderBackground_4;

    // Test Setting
    CurOrderCount = 2;


    float StartPos = 0.0f;

    for (int i = 0; i < Orders.Num(); i++)
    {
        Orders[i].BackgroundImage->SetRenderTranslation({ StartPos + ImageOffset, 0.0f });
    
        StartPos += 250;
    }

    for (int i = CurOrderCount; i < Orders.Num(); i++)
    {
        Orders[i].BackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
    }

}

void UCookingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InvalidateLayoutAndVolatility();

}


void UCookingWidget::OrderComplete()
{

    if (!Orders[CompleteOrderNum].BackgroundImage) return;

    if (Orders[CompleteOrderNum].BackgroundImage->GetVisibility() != ESlateVisibility::Collapsed)
    {
        CurOrderCount -= 1;

        GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
    }

}

void UCookingWidget::CreateNewOrder()
{
    NewOrderNum = CurOrderCount;
    if (NewOrderNum < Orders.Num())
    {
        FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);

        Orders[NewOrderNum].BackgroundImage->SetVisibility(ESlateVisibility::Visible);

        const FLinearColor& curcolor = Orders[NewOrderNum].BackgroundImage->GetColorAndOpacity();

        Orders[NewOrderNum].BackgroundImage->SetRenderTransformAngle(-20.0f);
        Orders[NewOrderNum].BackgroundImage->SetColorAndOpacity({ curcolor.R, curcolor.G, curcolor.B, 1.0f });
        Orders[NewOrderNum].BackgroundImage->SetRenderTranslation({ ScreenSize.X * 1.5f, 0.0f });
        Orders[NewOrderNum].BackgroundImage->SetRenderScale({ 1.0f, 1.2f });

        CurOrderCount += 1;

        GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
    }
}



void UCookingWidget::MoveNewOrder()
{

    if (!Orders[NewOrderNum].BackgroundImage) return;

    FVector2D curpos = Orders[NewOrderNum].BackgroundImage->GetRenderTransform().Translation;
    
    float curangle = Orders[NewOrderNum].BackgroundImage->GetRenderTransform().Angle;

    if (NewOrderNum != 0)
    {
        ArrivePos = ImageOffset;
        for (int i = 0; i < CurOrderCount-1; i++)
        {
            ArrivePos += Orders[i].BackgroundImage->GetDesiredSize().X;
        }

    }
    else if (NewOrderNum == 0)
    {
        ArrivePos = ImageOffset;
    }
    else
    {
        return;
    }

    if (curpos.X <= ArrivePos && curangle >= 0.0f)
    {
        Orders[NewOrderNum].BackgroundImage->SetRenderScale({1.0f, 1.0f});
        Orders[NewOrderNum].BackgroundImage->SetRenderTranslation({ ArrivePos,0.0f });
        Orders[NewOrderNum].BackgroundImage->SetRenderTransformAngle({ 0.0f });

        MoveTimeElapsed = 0.0f;
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;
    }
    else
    {
        if (curpos.X > ArrivePos)
        {
            Orders[NewOrderNum].BackgroundImage->SetRenderTranslation(curpos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));

        }
        if (curangle < 0.0f)
        {
            Orders[NewOrderNum].BackgroundImage->SetRenderTransformAngle(curangle + 0.7f);
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

    class UImage* backgroundimg = Orders[CompleteOrderNum].BackgroundImage;

    // 앞으로 이동
    for (int i = CompleteOrderNum; i < CurOrderCount; i++)
    {

        const FVector2D& imagesize = Orders[i].BackgroundImage->GetDesiredSize();
        int lastnum = 0;

        Orders[i] = Orders[i + 1];
        const FWidgetTransform& trfm = Orders[i].BackgroundImage->GetRenderTransform();
        FinalPos = trfm.Translation.X - imagesize.X ;

        Orders[i].BackgroundImage->SetRenderTranslation({ FinalPos, 0.0f });

    }

    Orders[CurOrderCount].BackgroundImage = backgroundimg;

    return;
    
}