// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"

void ULobbyMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Buttons.SetNum(5);
    Buttons = { StoryButton, ArcadeButton, BattleButton, ChefButton, OptionButton };

    for (int i = 0; i < Buttons.Num(); i++)
    {
        Buttons[i]->OnHovered.AddDynamic(this, &ULobbyMenuWidget::HoverButton);
    }

}


void ULobbyMenuWidget::HoverButton()
{
    for (UButton* Button : Buttons)
    {
        UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button);
        if (nullptr != Button && true == Button->IsHovered())
        {
            if (Panel)
            {
                Panel->SetVisibility(ESlateVisibility::Visible);
                if (CurPanel != Panel)
                {
                    CurPanel = Panel;
                    GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
                    GetWorld()->GetTimerManager().SetTimer(MenuMoveTimerHandle, this, &ULobbyMenuWidget::UpdateMenuPosition, 0.01f, true);
                }

            }
        }
        else
        {
            Panel->SetVisibility(ESlateVisibility::Hidden);
            Panel->SetRenderTranslation({ 0.0f, -82.0f });
        }
    }
}

void ULobbyMenuWidget::UpdateMenuPosition()
{
    if (CurPanel->GetRenderTransform().Translation.Y >= 0)
    {
        CurPanel->SetRenderTranslation({ 0.0f, 0.0f });

        GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
    }
    else
    {
        float CurPos = CurPanel->GetRenderTransform().Translation.Y;
        CurPanel->SetRenderTranslation({ 0.0f, CurPos + MenuMoveOffset });
    }
}


template <typename T>
T* ULobbyMenuWidget::FindSiblingWidget(UWidget* Widget)
{
    if (!Widget) return nullptr;

    UPanelWidget* Parent = Cast<UPanelWidget>(Widget->GetParent());
    if (!Parent) return nullptr;

    for (UWidget* Child : Parent->GetAllChildren())
    {
        if (T* TargetWidget = Cast<T>(Child))
        {
            return TargetWidget;
        }
    }

    return nullptr;
}