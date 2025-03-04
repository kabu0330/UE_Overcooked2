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


    Orders[0] = Order_0;
    Orders[1] = Order_1;
    Orders[2] = Order_2;
    Orders[3] = Order_3;
    Orders[4] = Order_4;


    // Test Setting
    CurOrderCount = 2;

    float StartPos = 0.0f;

    for (int i = 0; i < Orders.Num(); i++)
    {
        Orders[i]->SetRenderTranslation({ StartPos + ImageOffset * (i + 1), 0.0f });

        StartPos += ImageSize;
    }

    for (int i = CurOrderCount; i < Orders.Num(); i++)
    {
        Orders[i]->SetVisibility(ESlateVisibility::Collapsed);
    }

    {
        int testnum = 0;

        FName ImageName = *FString::Printf(TEXT("Dish_%d"), testnum);

        TArray<UWidget*> Children;
        Children = Orders[0]->GetAllChildren();

        for (UWidget* Child : Children)
        {
            if (UImage* Image = Cast<UImage>(Child))
            {

                if (Image->GetName() == ImageName)
                {
                    int a = 0;
                    //Image->SetBrushFromTexture();
                }
            }
        }
    }
}

void UCookingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InvalidateLayoutAndVolatility();

}


void UCookingWidget::OrderComplete()
{

    if (Orders[CompleteOrderNum] == nullptr) return;

    if (GetWorld()->GetTimerManager().IsTimerActive(OpacityTimerHandle)) return;

    if (Orders[CompleteOrderNum]->GetVisibility() != ESlateVisibility::Collapsed)
    {
        CurOrderCount -= 1;

        GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
    }

}

void UCookingWidget::CreateNewOrder()
{
    if (CurOrderCount >= Orders.Num()) return; 

    if (GetWorld()->GetTimerManager().IsTimerActive(MoveTimerHandle)) return;

    NewOrderNum = CurOrderCount;

    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);

    Orders[NewOrderNum]->SetVisibility(ESlateVisibility::Visible);
    Orders[NewOrderNum]->SetRenderTransformAngle(-20.0f);
    Orders[NewOrderNum]->SetRenderOpacity(1.0f);
    Orders[NewOrderNum]->SetRenderTranslation({ ScreenSize.X * 1.5f, 0.0f });
    Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.2f });

    CurOrderCount += 1;


    GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
}



void UCookingWidget::MoveNewOrder()
{

    if (Orders[NewOrderNum] == nullptr) return;

    FVector2D curpos = Orders[NewOrderNum]->GetRenderTransform().Translation;

    float curangle = Orders[NewOrderNum]->GetRenderTransform().Angle;

    if (NewOrderNum != 0)
    {
        ArrivePos = ImageOffset;
        for (int i = 0; i < CurOrderCount - 1; i++)
        {
            ArrivePos += ImageSize + ImageOffset;
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
        Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.0f });
        Orders[NewOrderNum]->SetRenderTranslation({ ArrivePos,0.0f });
        Orders[NewOrderNum]->SetRenderTransformAngle({ 0.0f });

        MoveTimeElapsed = 0.0f;

        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;
    }
    else
    {
        if (curpos.X > ArrivePos)
        {
            Orders[NewOrderNum]->SetRenderTranslation(curpos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));

        }
        if (curangle < 0.0f)
        {
            Orders[NewOrderNum]->SetRenderTransformAngle(curangle + 0.7f);
        }
    }

    MoveTimeElapsed += 0.1f;
}



void UCookingWidget::UpdateImageOpacity()
{

    if (Orders[CompleteOrderNum]->GetRenderOpacity() <= 0.0f)
    {
        Orders[CompleteOrderNum]->SetVisibility(ESlateVisibility::Collapsed);
        UpdateImagePosition();

        GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
        return;
    }

    float curcolor = Orders[CompleteOrderNum]->GetRenderOpacity();
    Orders[CompleteOrderNum]->SetRenderOpacity(FMath::Clamp(curcolor - OpacityOffset, 0.0f, 1.0f));

}



void UCookingWidget::UpdateImagePosition()
{

    if (CompleteOrderNum + 1 == Orders.Num())
    {
        return;
    }

    class UCanvasPanel* Panel = Orders[CompleteOrderNum];

    // 앞으로 이동
    for (int i = CompleteOrderNum; i < CurOrderCount; i++)
    {

        int lastnum = 0;

        Orders[i] = Orders[i + 1];
        const FWidgetTransform& trfm = Orders[i]->GetRenderTransform();
        FinalPos = trfm.Translation.X - ImageSize - ImageOffset;

        Orders[i]->SetRenderTranslation({ FinalPos, 0.0f });

    }

    Orders[CurOrderCount] = Panel;

    return;

}