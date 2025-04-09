// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyZoomInWidget.h"

#include "Components/Image.h" 
#include "Lobby/LobbyHUD.h"
#include "UI/Loading/LoadingWidget.h"
#include "Kismet/GameplayStatics.h"

void ULobbyZoomInWidget::NativeConstruct()
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


void ULobbyZoomInWidget::PlayZoomInAnimation(TFunction<void()> Func)
{
    if (!TransitionMaterial) return;
    TransitionImg->SetVisibility(ESlateVisibility::Visible);

    AnimFinishFuction = Func;

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
                APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
                ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());
                if (LobbyHUD != nullptr && LobbyHUD->LoadingWidget != nullptr)
                {
                    LobbyHUD->LoadingWidget->SetVisibility(ESlateVisibility::Visible);
                    LobbyHUD->LoadingWidget->PlayLoadingAnimation(AnimFinishFuction, ELevelChangType::LobbyToWorldMap);
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
