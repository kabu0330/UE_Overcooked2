// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/LobbyMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Slate/SlateBrushAsset.h"
#include "Components/AudioComponent.h"

#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Global/Data/OC2GlobalData.h"

void ULobbyMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Buttons = { StoryButton, ArcadeButton, BattleButton, ChefButton, OptionButton };

    CurPanel = FindSiblingWidget<UCanvasPanel>(StoryButton);
    CurButtonNomalBrush = StoryButton->GetStyle().Normal;

    FButtonStyle NewStyle = StoryButton->GetStyle();
    NewStyle.Normal = NewStyle.Hovered;
    StoryButton->SetStyle(NewStyle);
    CurMainButton = StoryButton;

    for (int i = 0; i < Buttons.Num(); i++)
    {
        if (Buttons[i])
        {
            Buttons[i]->OnHovered.AddDynamic(this, &ULobbyMenuWidget::HoverButton);

            if (UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Buttons[i]))
            {
                TArray<UButton*> AllButtons = FindAllChildWidgets<UButton>(Panel);
                for (UButton* SubButton : AllButtons)
                {
                    if (SubButton)
                    {
                        SubButton->OnHovered.AddDynamic(this, &ULobbyMenuWidget::HoverSubButton);
                    }
                }
            }
        }
    }
}

void ULobbyMenuWidget::NativeDestruct()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
    }
    Super::NativeDestruct();
}

void ULobbyMenuWidget::HoverButton()
{
    for (UButton* Button : Buttons)
    {
        if (!Button) continue;

        UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button);
        if (Button->IsHovered())
        {
            if (Panel)
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

                    // 사운드 안전 체크
                    if (USoundBase* MenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleMenuButtonSound"))
                    {
                        UGameplayStatics::SpawnSound2D(this, MenuButtonSound);
                    }

                    GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
                    GetWorld()->GetTimerManager().SetTimer(MenuMoveTimerHandle, this, &ULobbyMenuWidget::UpdateMenuPosition, 0.01f, true);
                }
            }
        }
        else
        {
            if (Panel)
            {
                Panel->SetVisibility(ESlateVisibility::Hidden);
                Panel->SetRenderTranslation({ 0.0f, -82.0f });
            }
        }
    }
}

void ULobbyMenuWidget::HoverSubButton()
{
    if (!CurPanel) return;

    TArray<UButton*> AllButtons = FindAllChildWidgets<UButton>(CurPanel);

    for (UButton* SubButton : AllButtons)
    {
        if (!SubButton || !SubButton->IsHovered()) continue;

        if (CurButton != SubButton)
        {
            CurButton = SubButton;

            if (USoundBase* SubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound"))
            {
                UGameplayStatics::SpawnSound2D(this, SubMenuButtonSound);
            }
        }
        else if (!bIsCurButton)
        {
            if (USoundBase* SubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound"))
            {
                UGameplayStatics::SpawnSound2D(this, SubMenuButtonSound);
            }

            bIsCurButton = true;
        }
    }
}

void ULobbyMenuWidget::UpdateMenuPosition()
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
T* ULobbyMenuWidget::FindSiblingWidget(UWidget* Widget)
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
TArray<T*> ULobbyMenuWidget::FindAllChildWidgets(UCanvasPanel* Panel)
{
    TArray<T*> FoundWidgets;

    if (!Panel) return FoundWidgets;

    for (UWidget* Child : Panel->GetAllChildren())
    {
        if (T* Widget = Cast<T>(Child))
        {
            FoundWidgets.Add(Widget);
        }
    }

    return FoundWidgets;
}
