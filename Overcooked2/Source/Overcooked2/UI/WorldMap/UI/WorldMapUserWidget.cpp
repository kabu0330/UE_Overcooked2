// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldMap/UI/WorldMapUserWidget.h"
#include "Components/Image.h"

void UWorldMapUserWidget::NativeConstruct()
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


void UWorldMapUserWidget::PlayZoomInAnimation()
{
    if (!TransitionMaterial) return;
    TransitionImg->SetVisibility(ESlateVisibility::Visible);

    float AnimationDuration = 20.0f; // 전체 애니메이션 길이 (1초)
    float TimeStep = 0.01f; // 0.01초(10ms)마다 업데이트
    float CurrentTime = 0.0f; // 현재 애니메이션 진행 시간

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime >= AnimationDuration)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);
            }

            float Value1 = CurrentTime; // 애니메이션 진행 정도 (0 ~ 1)
            float Value2 = (Value1 - 1.0f) / 2.0f; // 공식 적용

            TransitionMaterial->SetScalarParameterValue(TEXT("Value1"), Value1);
            TransitionMaterial->SetScalarParameterValue(TEXT("Value2"), Value2);

            if (CurrentTime < 5.0f)
            {
                CurrentTime += TimeStep * 20.0f;
            }
            else
            {
                CurrentTime += TimeStep * 40.0f; // 시간 업데이트
            }


        }, TimeStep, true);
}