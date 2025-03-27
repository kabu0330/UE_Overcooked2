// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleMenuWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Slate/SlateBrushAsset.h"


void UTitleMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Buttons.SetNum(5);
    Buttons = { StoryButton, ArcadeButton, BattleButton, ChefButton, OptionButton };

    for (int i = 0; i < Buttons.Num(); i++)
    {
        Buttons[i]->OnHovered.AddDynamic(this, &UTitleMenuWidget::HoverButton);
    }

}


void UTitleMenuWidget::HoverButton()
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
                    GetWorld()->GetTimerManager().SetTimer(MenuMoveTimerHandle, this, &UTitleMenuWidget::UpdateMenuPosition, 0.01f, true);
                }

            }
        }
        else
        {
            Panel->SetVisibility(ESlateVisibility::Hidden);
            Panel->SetRenderTranslation({0.0f, -82.0f});
        }
    }
}

void UTitleMenuWidget::UpdateMenuPosition()
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
T* UTitleMenuWidget::FindSiblingWidget(UWidget* Widget)
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




template <typename T>
T* UTitleMenuWidget::FindChildWidget(const FString& Name, UCanvasPanel* Canvas)
{
    if (!Canvas) return nullptr;

    FString TargetPrefix = Name;
    TArray<UWidget*> Children = Canvas->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (T* Widget = Cast<T>(Child))
        {
            if (Widget->GetName().StartsWith(TargetPrefix))
            {
                return Widget;
            }
        }
    }

    return nullptr;
}
