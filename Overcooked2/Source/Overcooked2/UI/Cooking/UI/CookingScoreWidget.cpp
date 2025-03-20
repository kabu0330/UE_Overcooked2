// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingScoreWidget.h"
#include "Components/Image.h"

void UCookingScoreWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UImage* CoinImg = Cast<UImage>(GetWidgetFromName(TEXT("CoinImage")));
    if (CoinImg != nullptr)
    {
        CoinMaterial = CoinImg->GetDynamicMaterial();

        if (!CoinMaterial)
        {
            CoinMaterial = CoinImg->GetDynamicMaterial();
        }
    }
}

void UCookingScoreWidget::PlayCoinAnimation()
{
    if (!CoinMaterial) return;

    float AnimationDuration = 1.0f;
    float TimeStep = 0.01f;
    float CurrentTime = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime >= AnimationDuration)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);
                return;
            }

            CoinMaterial->SetScalarParameterValue(TEXT("CoinTime"), CurrentTime);
            CurrentTime += TimeStep;

        }, TimeStep, true);
}
