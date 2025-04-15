// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleSubmenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"

#include "Sound/SoundBase.h" 
#include "Kismet/GameplayStatics.h" 
#include "Global/Data/OC2GlobalData.h"
#include "Components/AudioComponent.h"

void UTitleSubmenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    IP_Panel->SetRenderTranslation({ 0.0f, -500.0f });
    IP_Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UTitleSubmenuWidget::UpdateBannerPosition, 0.01f, true);
}

void UTitleSubmenuWidget::UpdateBannerPosition()
{     

    if (IP_Panel->GetRenderTransform().Translation.Y >= 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
    }
    MoveYOffset = IP_Panel->GetRenderTransform().Translation.Y;
    MoveYOffset += 30.0f;
    IP_Panel->SetRenderTranslation({ 0.0f, MoveYOffset });
}


void UTitleSubmenuWidget::MoveBanner()
{
    IP_Panel->SetRenderTranslation({ 0.0f, -500.0f });
    IP_Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UTitleSubmenuWidget::UpdateBannerPosition, 0.01f, true);
}

void UTitleSubmenuWidget::PlayHoverSound()
{
    USoundBase* TitleSubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound");
    UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, TitleSubMenuButtonSound);
}

void UTitleSubmenuWidget::PlayPressSound()
{
    USoundBase* TitleSubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound");
    UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, TitleSubMenuButtonSound);
}
