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



void UTitleMenuWidget::SettingTitleMenu()
{
      for (int i = 0; i < Buttons.Num(); i++)
    {
        Buttons[i]->OnHovered.AddDynamic(this, &UTitleMenuWidget::HoverButton);

        UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Buttons[i]);
        if (Panel)
        {
            TArray<UButton*> AllButtons = FindAllChildWidgets<UButton>(Panel);

            for (UButton* SubButton : AllButtons)
            {
                SubButton->OnHovered.AddDynamic(this, &UTitleMenuWidget::HoverSubButton);
                //SubButton->OnUnhovered
            }
        }
    }

    USoundBase* TitleSubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound");
    TitleSubMenuAudioComp = NewObject<UAudioComponent>(this);
    TitleSubMenuAudioComp->SetSound(TitleSubMenuButtonSound);
    TitleSubMenuAudioComp->RegisterComponentWithWorld(GetWorld());


    USoundBase* TitleMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleMenuButtonSound");
    MenuButtonAudioComp = NewObject<UAudioComponent>(this);
    MenuButtonAudioComp->SetSound(TitleMenuButtonSound);
    MenuButtonAudioComp->RegisterComponentWithWorld(GetWorld());


}

void UTitleMenuWidget::HoverSubButton()
{
    if (nullptr == CurPanel) return;

    TArray<UButton*> AllButtons = FindAllChildWidgets<UButton>(CurPanel);

    for (UButton* SubButton : AllButtons)
    {
        //bIsCurPanel = false;
        if (CurButton != SubButton && SubButton->IsHovered())
        {
            CurButton = SubButton;

            if (nullptr != TitleSubMenuAudioComp)
            {
                TitleSubMenuAudioComp->Play();
            }

        }
        else if (CurButton == SubButton && SubButton->IsHovered() && false == bIsCurButton)
        {
            if (nullptr != TitleSubMenuAudioComp)
            {
                TitleSubMenuAudioComp->Play();
            }
            bIsCurButton = true;
            
        }

    }
}

void UTitleMenuWidget::HoverButton()
{
    for (UButton* Button : Buttons)
    {
        UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Button);
        if (nullptr != Button && true == Button->IsHovered()) 
        {
            if (nullptr != Panel)
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

                    if (nullptr != MenuButtonAudioComp)
                    {
                        MenuButtonAudioComp->Play();
                    }


                    
                    GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
                    GetWorld()->GetTimerManager().SetTimer(MenuMoveTimerHandle, this, &UTitleMenuWidget::UpdateMenuPosition, 0.01f, true);
                }

            }
        }
        else
        {
            if (nullptr != Panel)
            {
                Panel->SetVisibility(ESlateVisibility::Hidden);
                Panel->SetRenderTranslation({ 0.0f, -82.0f });
            }

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
TArray<T*> UTitleMenuWidget::FindAllChildWidgets(UCanvasPanel* Panel)
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
