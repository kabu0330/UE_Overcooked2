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

    Buttons.SetNum(5);
    Buttons = { StoryButton, ArcadeButton, BattleButton, ChefButton, OptionButton };

    CurPanel = FindSiblingWidget<UCanvasPanel>(StoryButton);
    CurButtonNomalBrush = StoryButton->GetStyle().Normal;

    FButtonStyle NewStyle = StoryButton->GetStyle();
    NewStyle.Normal = NewStyle.Hovered;
    StoryButton->SetStyle(NewStyle);
    CurMainButton = StoryButton;

    for (int i = 0; i < Buttons.Num(); i++)
    {
        Buttons[i]->OnHovered.AddDynamic(this, &ULobbyMenuWidget::HoverButton);

        UCanvasPanel* Panel = FindSiblingWidget<UCanvasPanel>(Buttons[i]);
        if (Panel)
        {
            TArray<UButton*> AllButtons = FindAllChildWidgets<UButton>(Panel);

            for (UButton* SubButton : AllButtons)
            {
                SubButton->OnHovered.AddDynamic(this, &ULobbyMenuWidget::HoverSubButton);
                //SubButton->OnUnhovered
            }
        }
    }




}

void ULobbyMenuWidget::HoverButton()
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


                    USoundBase* MenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleMenuButtonSound");;
                    UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, MenuButtonSound);

                    GetWorld()->GetTimerManager().ClearTimer(MenuMoveTimerHandle);
                    GetWorld()->GetTimerManager().SetTimer(MenuMoveTimerHandle, this, &ULobbyMenuWidget::UpdateMenuPosition, 0.01f, true);
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

void ULobbyMenuWidget::HoverSubButton()
{
    if (nullptr == CurPanel) return;

    TArray<UButton*> AllButtons = FindAllChildWidgets<UButton>(CurPanel);

    for (UButton* SubButton : AllButtons)
    {
        //bIsCurPanel = false;
        if (CurButton != SubButton && SubButton->IsHovered())
        {
            CurButton = SubButton;

            USoundBase* SubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound");
            UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, SubMenuButtonSound);
        }
        else if (CurButton == SubButton && SubButton->IsHovered() && false == bIsCurButton)
        {
            USoundBase* SubMenuButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSubMenuButtonSound");
            UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, SubMenuButtonSound);
            bIsCurButton = true;

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
