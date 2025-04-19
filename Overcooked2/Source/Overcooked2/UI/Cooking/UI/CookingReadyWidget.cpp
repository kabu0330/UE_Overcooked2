// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingReadyWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundBase.h" 
#include "Global/Data/OC2GlobalData.h"
#include "Components/AudioComponent.h"

void UCookingReadyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ReadyProgress != nullptr)
    {
        ProgressMaterial = ReadyProgress->GetDynamicMaterial();
    }

    //if (GetWorld()->GetAuthGameMode())
    //{
    //    ReadyProgress->SetVisibility(ESlateVisibility::Visible);
    //    SpaceBarImg->SetVisibility(ESlateVisibility::Visible);
    //    SkipText->SetVisibility(ESlateVisibility::Visible);
    //}
    //else
    //{
    //    ReadyProgress->SetVisibility(ESlateVisibility::Collapsed);
    //    SpaceBarImg->SetVisibility(ESlateVisibility::Collapsed);
    //    SkipText->SetVisibility(ESlateVisibility::Collapsed);
    //}


    if (TransitionImg != nullptr)
    {
        TransitionMaterial = TransitionImg->GetDynamicMaterial();
        TransitionImg->SetVisibility(ESlateVisibility::Hidden);
    }

    if (HoldCanvas != nullptr)
    {
        HoldCanvas->SetVisibility(ESlateVisibility::Hidden);
    }
    //bIsReady = true;
    PlayZoomOutAnimation();
}

void UCookingReadyWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);
    //if (HoldCanvas->GetVisibility() == ESlateVisibility::Hidden) return;

    //APlayerController* PC = GetOwningPlayer();
    //if (!PC || !PC->HasAuthority()) return;

    //if (ProgressTime >= 1.0f)
    //{
    //    bIsReady = true;
    //}

    //if (bHoldingSpace == true )
    //{
    //    //ProgressTime = FMath::Clamp(ProgressTime + DeltaTime * HoldSpeed, 0.f, 1.f);
    //}
    //else if(bHoldingSpace == false && ProgressTime < 1.0f)
    //{
    //    ProgressTime = 0.0f;
    //}

   
}

void UCookingReadyWidget::SetHoldProgress(int Progress)
{
    ProgressTime = FMath::Clamp(Progress / 100.f, 0.f, 1.f);
    ProgressMaterial->SetScalarParameterValue(TEXT("Percent"), ProgressTime);
}


void UCookingReadyWidget::PlayZoomOutAnimation()
{
    if (!TransitionMaterial) return;

    TransitionImg->SetVisibility(ESlateVisibility::Visible);

    float AnimationDuration = 10.0f;
    float TimeStep = 0.01f;
    float CurrentTime = AnimationDuration;


    if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ScreenOut"))
    {
        UGameplayStatics::PlaySound2D(this, Sound);
    }

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime <= 0.0f)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);
                //TransitionImg->SetVisibility(ESlateVisibility::Hidden);

                return;
            }

            float Value1 = CurrentTime;
            float Value2 = (Value1 - 1.0f) / 2.0f;

            if (IsValid( TransitionMaterial))
            {
                TransitionMaterial->SetScalarParameterValue(TEXT("Value1"), Value1);
                TransitionMaterial->SetScalarParameterValue(TEXT("Value2"), Value2);
            }


            CurrentTime -= TimeStep * 15.0f;

        }, TimeStep, true);

}
