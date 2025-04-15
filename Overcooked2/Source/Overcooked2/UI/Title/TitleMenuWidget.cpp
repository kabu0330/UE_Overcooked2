// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Title/TitleMenuWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/AudioComponent.h"
#include "Engine/Texture2D.h"
#include "Slate/SlateBrushAsset.h"

#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Global/Data/OC2GlobalData.h"

void UTitleMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Buttons = { StoryButton, ArcadeButton, BattleButton, ChefButton, OptionButton };

    CurPanel = FindSiblingWidget<UCanvasPanel>(StoryButton);
    CurButtonNomalBrush = StoryButton->GetStyle().Normal;

    FButtonStyle NewStyle = StoryButton->GetStyle();
    NewStyle.Normal = NewStyle.Hovered;
    StoryButton->SetStyle(NewStyle);
    CurMainButton = StoryButton;
}

void UTitleMenuWidget::NativeDestruct()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
    }
    Super::NativeDestruct();
}

void UTitleMenuWidget::SettingTitleMenu()
{
    for (UButton* Button : Buttons)
    {
        if (!Button) continue;

        Button->OnHovered.AddDynamic(this, &UTitleMenuWidget::HoverButton);

        if (UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button))
        {
            TArray<UButton*> SubButtons = FindAllChildWidgets<UButton>(Panel);
            for (UButton* SubButton : SubButtons)
            {
                if (SubButton)
                {
                    SubButton->OnHovered.AddDynamic(this, &UTitleMenuWidget::HoverSubButton);
                }
            }
        }
    }
}

void UTitleMenuWidget::HoverSubButton()
{
    if (!CurPanel) return;

    TArray<UButton*> SubButtons = FindAllChildWidgets<UButton>(CurPanel);

    for (UButton* SubButton : SubButtons)
    {
        if (!SubButton || !SubButton->IsHovered()) continue;

        if (CurButton != SubButton)
        {
            CurButton = SubButton;

            if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound"))
            {
                UGameplayStatics::SpawnSound2D(this, Sound);
            }
        }
        else if (!bIsCurButton)
        {
            if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound"))
            {
                UGameplayStatics::SpawnSound2D(this, Sound);
            }

            bIsCurButton = true;
        }
    }
}

void UTitleMenuWidget::HoverButton()
{
    for (UButton* Button : Buttons)
    {
        if (!Button) continue;

        if (Button->IsHovered())
        {
            if (UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button))
            {
                Panel->SetVisibility(ESlateVisibility::Visible);
                bIsCurButton = false;

                if (CurPanel != Panel)
                {
                    CurPanel = Panel;

                    if (CurMainButton)
                    {
                        FButtonStyle OldStyle = CurMainButton->GetStyle();
                        OldStyle.Normal = CurButtonNomalBrush;
                        CurMainButton->SetStyle(OldStyle);
                    }

                    CurMainButton = Button;
                    CurButtonNomalBrush = Button->GetStyle().Normal;

                    FButtonStyle NewStyle = Button->GetStyle();
                    NewStyle.Normal = NewStyle.Hovered;
                    Button->SetStyle(NewStyle);

                    if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleMenuButtonSound"))
                    {
                        UGameplayStatics::SpawnSound2D(this, Sound);
                    }

                    GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
                    GetWorld()->GetTimerManager().SetTimer(MenuMoveTimerHandle, this, &UTitleMenuWidget::UpdateMenuPosition, 0.01f, true);
                }
            }
        }
        else
        {
            if (UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button))
            {
                Panel->SetVisibility(ESlateVisibility::Hidden);
                Panel->SetRenderTranslation({ 0.0f, -82.0f });
            }
        }
    }
}

void UTitleMenuWidget::UpdateMenuPosition()
{
    if (!CurPanel) return;

    float CurY = CurPanel->GetRenderTransform().Translation.Y;

    if (CurY >= 0.0f)
    {
        CurPanel->SetRenderTranslation({ 0.0f, 0.0f });
        GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
    }
    else
    {
        CurPanel->SetRenderTranslation({ 0.0f, CurY + MenuMoveOffset });
    }
}

template <typename T>
T* UTitleMenuWidget::FindSiblingWidget(UWidget* Widget)
{
    if (!Widget) return nullptr;

    if (UPanelWidget* Parent = Cast<UPanelWidget>(Widget->GetParent()))
    {
        for (UWidget* Child : Parent->GetAllChildren())
        {
            if (T* TargetWidget = Cast<T>(Child))
            {
                return TargetWidget;
            }
        }
    }

    return nullptr;
}

template <typename T>
TArray<T*> UTitleMenuWidget::FindAllChildWidgets(UCanvasPanel* Panel)
{
    TArray<T*> Result;

    if (!Panel) return Result;

    for (UWidget* Child : Panel->GetAllChildren())
    {
        if (T* Widget = Cast<T>(Child))
        {
            Result.Add(Widget);
        }
    }

    return Result;
}
