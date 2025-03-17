// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleMenuWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Slate/SlateBrushAsset.h"


void UTitleMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Buttons.SetNum(2);
    Buttons = { StoryButton, ArcadeButton };

    for (int i = 0; i < Buttons.Num(); i++)
    {
        Buttons[i]->OnHovered.AddDynamic(this, &UTitleMenuWidget::OnButtonHovered);
    }

}


void UTitleMenuWidget::OnButtonHovered()
{
    for (UButton* Button : Buttons)
    {
        UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button);
        if (nullptr != Button && true == Button->IsHovered()) 
        {
            if (Panel)
            {
                Panel->SetVisibility(ESlateVisibility::Visible);
            }
        }
        else
        {
            Panel->SetVisibility(ESlateVisibility::Hidden);
        }
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
