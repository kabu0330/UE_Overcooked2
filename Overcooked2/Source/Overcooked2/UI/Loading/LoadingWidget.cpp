// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/LoadingWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h" 

void ULoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();


    if (TransitionImg != nullptr)
    {
        TransitionMaterial = TransitionImg->GetDynamicMaterial();

        if (!TransitionMaterial)
        {
            TransitionMaterial = TransitionImg->GetDynamicMaterial();
        }
    }
    TransitionImg->SetVisibility(ESlateVisibility::Hidden);

}
void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);


    if (bIsLoadingStart)
    {
        ProgressTime += DeltaTime;

        float ProgressRatio = FMath::Clamp(ProgressTime / 5.0f, 0.0f, 1.0f);
        SetProgress(ProgressRatio);

        if (ProgressRatio >= 1.0f)
        {
            bIsLoadingStart = false;
            PlayZoomInAnimation();
        }
    }
}


void ULoadingWidget::SetProgress(float Value)
{
    if (LoadingProgressBar != nullptr)
    {
        LoadingProgressBar->SetPercent(Value);
    }
}


void ULoadingWidget::PlayLoadingAnimation(TFunction<void()> Func)
{

    float CurrentTime = 0.0f;
    float TimeStep = 0.1f;

    TFunction<void()> Function = Func;
    GetWorld()->GetTimerManager().ClearTimer(LoadingAnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(LoadingAnimationTimer, [this, CurrentTime, TimeStep, Function]() mutable
        {
            if (CurrentTime >= 6.0f)
            {
                GetWorld()->GetTimerManager().ClearTimer(LoadingAnimationTimer);
                PlayZoomInAnimation();
                Function();
                return;
            }
            float ProgressRatio = FMath::Clamp(CurrentTime / 5.0f, 0.0f, 1.0f); // 5ÃÊ ±âÁØ
            SetProgress(ProgressRatio);
            CurrentTime += TimeStep;

        }, TimeStep, true);

}



void ULoadingWidget::PlayZoomOutAnimation()
{
    if (!TransitionMaterial) return;

    TransitionImg->SetVisibility(ESlateVisibility::Visible);

    float AnimationDuration = 10.0f;
    float TimeStep = 0.01f;
    float CurrentTime = AnimationDuration;

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime <= 0.0f)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);
                TransitionImg->SetVisibility(ESlateVisibility::Hidden);

                return;
            }

            float Value1 = CurrentTime;
            float Value2 = (Value1 - 1.0f) / 2.0f;

            TransitionMaterial->SetScalarParameterValue(TEXT("Value1"), Value1);
            TransitionMaterial->SetScalarParameterValue(TEXT("Value2"), Value2);

            CurrentTime -= TimeStep * 20.0f;

        }, TimeStep, true);

}


void ULoadingWidget::PlayZoomInAnimation()
{
    if (!TransitionMaterial) return;
    TransitionImg->SetVisibility(ESlateVisibility::Visible);

    //AnimFinishFuction = Func;

    float AnimationDuration = 20.0f;
    float TimeStep = 0.01f;
    float CurrentTime = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime >= AnimationDuration)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

                //TransitionImg->SetVisibility(ESlateVisibility::Hidden);

                //if (AnimFinishFuction)
                {
                    //     AnimFinishFuction();
                }

                return;
            }

            float Value1 = CurrentTime;
            float Value2 = (Value1 - 1.0f) / 2.0f;

            TransitionMaterial->SetScalarParameterValue(TEXT("Value1"), Value1);
            TransitionMaterial->SetScalarParameterValue(TEXT("Value2"), Value2);

            if (Value2 < 0.0f)
            {
                CurrentTime += TimeStep * 20.0f;
            }
            else
            {
                CurrentTime += TimeStep * 40.0f;
            }
        }, TimeStep, true);
}
