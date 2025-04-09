// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/LoadingWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h" 
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
    if (ConnectingImage != nullptr)
    {
        ConnectingMaterial = ConnectingImage->GetDynamicMaterial();

        if (!ConnectingMaterial)
        {
            ConnectingMaterial = ConnectingImage->GetDynamicMaterial();
        }
    }
    
    TransitionImg->SetVisibility(ESlateVisibility::Hidden);

}


void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    if (bIsConnecting == true)
    {
        CurTime += DeltaTime;
        if (CurTime >= 0.02f)
        {
            if (CurPanelType == ELevelChangType::WorldMapToSushi)
            {
                if (CurIndex >= BugerAnimationTotalIndex)
                {
                    CurIndex = 0.0f;
                }
            }
            else
            {
                if (CurIndex >= PizzaAnimationTotalIndex)
                {
                    CurIndex = 0.0f;
                }
            }


            ConnectingMaterial->SetScalarParameterValue(TEXT("ConnectingTime"), CurIndex);
            CurIndex += 0.01;
            CurTime = 0.0f;
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


void ULoadingWidget::PlayLoadingAnimation(TFunction<void()> Func, ELevelChangType LevelEnum)
{

    float CurrentTime = 0.0f;
    float TimeStep = 0.1f;
    CurPanelType = LevelEnum;
    if (LevelEnum == ELevelChangType::LobbyToWorldMap)
    {
        LobbyToWorldMap->SetVisibility(ESlateVisibility::Visible);
        WorldMapToSushiLevel->SetVisibility(ESlateVisibility::Hidden);


        UMaterialInterface* PizzaMaterial = PizzaMaterialAsset.LoadSynchronous();
        if (PizzaMaterial)
        {
            ConnectingMaterial = UMaterialInstanceDynamic::Create(PizzaMaterial, this);
            FSlateBrush Brush;
            Brush.SetResourceObject(ConnectingMaterial);
            ConnectingImage->SetBrush(Brush);
        }

    }
    else if (LevelEnum == ELevelChangType::WorldMapToSushi)
    {
        LobbyToWorldMap->SetVisibility(ESlateVisibility::Hidden);
        WorldMapToSushiLevel->SetVisibility(ESlateVisibility::Visible);

        UMaterialInterface* BurgerMaterial = BugerMaterialAsset.LoadSynchronous();
        if (BurgerMaterial)
        {
            ConnectingMaterial = UMaterialInstanceDynamic::Create(BurgerMaterial, this);
            FSlateBrush Brush;
            Brush.SetResourceObject(ConnectingMaterial);
            ConnectingImage->SetBrush(Brush);
        }
    }




    PlayZoomOutAnimation();
    Function = Func;

    GetWorld()->GetTimerManager().ClearTimer(LoadingAnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(LoadingAnimationTimer, [this, CurrentTime, TimeStep]() mutable
        {
            if (CurrentTime >= 3.0f)
            {
                GetWorld()->GetTimerManager().ClearTimer(LoadingAnimationTimer);
                PlayZoomInAnimation();
                return;
            }
            float ProgressRatio = FMath::Clamp(CurrentTime / 3.0f, 0.0f, 1.0f); // 5ÃÊ ±âÁØ
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

    float AnimationDuration = 20.0f;
    float TimeStep = 0.01f;
    float CurrentTime = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime >= AnimationDuration)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);
                ConnectingCanvas->SetVisibility(ESlateVisibility::Visible);
                bIsConnecting = true;
                





                Function();
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
