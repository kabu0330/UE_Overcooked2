// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingReceiptWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

void UCookingReceiptWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("PlayStarAnimaion"));
    StarTimeline.AddInterpFloat(StarCurve, ProgressFunction);

    FOnTimelineEvent FinishEvent;
    FinishEvent.BindUFunction(this, FName("ResetSize"));
    StarTimeline.SetTimelineFinishedFunc(FinishEvent);

}

void UCookingReceiptWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    StarTimeline.TickTimeline(DeltaTime);
}

void UCookingReceiptWidget::CheckStar()
{

    StarTimeline.SetLooping(false);
    StarTimeline.SetPlayRate(1.0f / 0.5f);
    StarTimeline.Stop();
    StarTimeline.PlayFromStart();

}

void UCookingReceiptWidget::ShowScoreText()
{
    GetWorld()->GetTimerManager().ClearTimer(ScoreTextTimerHandle);

    GetWorld()->GetTimerManager().SetTimer(ScoreTextTimerHandle, this, &UCookingReceiptWidget::PlayScoreTextAnimation, 0.5f, true);

}

void UCookingReceiptWidget::PlayScoreTextAnimation()
{
    if (CurTime == 0)
    {
        UTextBlock* DeliveredTxt = FindChildWidget<UTextBlock>("DeliveredTxt", TxtCanvas);
        UTextBlock* DeliveredCount = FindChildWidget<UTextBlock>("DeliveredTxt", TxtCanvas);

        DeliveredTxt->SetVisibility(ESlateVisibility::Visible);
        DeliveredCount->SetVisibility(ESlateVisibility::Visible);
    }
    else if (CurTime == 1)
    {
        UTextBlock* DeliveredScore = FindChildWidget<UTextBlock>("DeliveredScore", TxtCanvas);
        DeliveredScore->SetVisibility(ESlateVisibility::Visible);
    }
    else if (CurTime == 2)
    {
        UTextBlock* TipsTxt = FindChildWidget<UTextBlock>("TipsTxt", TxtCanvas);
        TipsTxt->SetVisibility(ESlateVisibility::Visible);
    }
    else if (CurTime == 3)
    {
        UTextBlock* TipScore = FindChildWidget<UTextBlock>("TipScore", TxtCanvas);
        TipScore->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(ScoreTextTimerHandle);

    }

    CurTime++;
}

void UCookingReceiptWidget::PlayStarAnimaion(float Value)
{
 
    FVector2D CurSize = FMath::Lerp(FVector2D(0.0f, 0.0f), FVector2D(2.0f, 2.0f), Value);

    UpdateStar(CurIndex, CurSize);
}

void UCookingReceiptWidget::UpdateStar(int Count, FVector2D Size)
{
    if (ReceiptCanvas != nullptr)
    {
        UImage* StarImage = FindChildWidget<UImage>("Star_" + FString::FromInt(Count), ReceiptCanvas);
        StarImage->SetVisibility(ESlateVisibility::Visible);
        StarImage->SetRenderScale(Size);
        CurImg = StarImage;
    }
}

void UCookingReceiptWidget::ResetSize()
{
    CurImg->SetRenderScale(FVector2D(1.0f, 1.0f));
    CurIndex += 1;

    int TestNum = 3;

    if(CurIndex < TestNum)
    {
        StarTimeline.Stop();
        StarTimeline.PlayFromStart();
    }
    else if (CurIndex == TestNum)
    {
        CurIndex = 0;
    }
}

template <typename T>
T* UCookingReceiptWidget::FindChildWidget(const FString& Name, UCanvasPanel* Canvas)
{
    if (!Canvas) return nullptr;

    FString TargetPrefix = Name;
    TArray<UWidget*> Children = Canvas->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (T* Widget = Cast<T>(Child))
        {
            if (Widget->GetName().StartsWith(TargetPrefix))
            {
                return Widget;
            }
        }
    }

    return nullptr;
}

