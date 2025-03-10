// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyUserWidget.h"
#include "Global/OC2Global.h"
#include "Character/OC2Character.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"



void ULobbyUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GetWorld()->GetAuthGameMode())
    {
        // 서버는 게임 시작 버튼만 보이게 설정
        if (StartGameButton)
            StartGameButton->SetVisibility(ESlateVisibility::Visible);
        if (ReadyButton)
            ReadyButton->SetVisibility(ESlateVisibility::Collapsed);

    }
    else
    {
        // 클라이언트는 준비 버튼만 보이게 설정
        if (StartGameButton)
            StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
        if (ReadyButton)
            ReadyButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyUserWidget::MoveToPlayLevel()
{
	// Only server
	if (GetWorld()->GetAuthGameMode())
	{
		UOC2Global::TravelServer(GetWorld(), PLAY_LEVEL);
	}
}

void ULobbyUserWidget::Ready()
{
    if (GetWorld()->GetAuthGameMode()) // 서버에서는 실행 안 함
    {
        return;
    }

}


template <typename T>
T* ULobbyUserWidget::FindChildWidget(const FString& Name, UCanvasPanel* Canvas)
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
