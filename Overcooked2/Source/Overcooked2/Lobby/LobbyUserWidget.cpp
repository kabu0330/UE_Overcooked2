// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyUserWidget.h"
#include "Global/OC2Global.h"
#include "Character/OC2Character.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Lobby/LobbyHUD.h"
#include "UI/Lobby/LobbyZoomInWidget.h"
#include "UI/Loading/LoadingWidget.h"
#include "Kismet/GameplayStatics.h"


void ULobbyUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GetWorld()->GetAuthGameMode())
    {
        // 서버는 게임 시작 버튼만 보이게 설정
        if (StartGameButton)
        {
            StartGameButton->SetVisibility(ESlateVisibility::Visible);
            ButtonImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }

        //if (ReadyButton)
        //    ReadyButton->SetVisibility(ESlateVisibility::Collapsed);

    }
    else
    {
        // 클라이언트는 준비 버튼만 보이게 설정
        if (StartGameButton)
        {
            StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
            ButtonImg->SetVisibility(ESlateVisibility::Collapsed);

        }

        //if (ReadyButton)
        //    ReadyButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyUserWidget::MoveToPlayLevel()
{


	// Only server
	if (GetWorld()->GetAuthGameMode())
	{

        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());

        if (LobbyHUD->LobbyZoomInWidget != nullptr && LobbyHUD != nullptr)
        {
            LobbyHUD->LobbyZoomInWidget->PlayZoomInAnimation([this]()
                {
                    UOC2Global::TravelServer(GetWorld(), PLAY_LEVEL);
                });
        }

	}
}

void ULobbyUserWidget::Ready()
{
    if (GetWorld()->GetAuthGameMode()) // 서버에서는 실행 안 함
    {
        return;
    }

}


void ULobbyUserWidget::SetUserTexture(UTexture2D* Texture, int Index)
{

    UImage* PlayersImage = FindChildWidget<UImage>("Player_" + FString::FromInt(Index), LobbyCanvasPanel);
    if (PlayersImage == nullptr) return;

    UImage*  PlayersBgImage = FindChildWidget<UImage>("PlayerBgImg_" + FString::FromInt(Index), LobbyCanvasPanel);
    if (PlayersBgImage == nullptr) return;

    UImage* AddPlayersBgImage = nullptr;

    {
        FString texturepath = TEXT("/Game/Resources/UI/Lobby/t_ui_multi_kitchen_gamer_tag_1_d.t_ui_multi_kitchen_gamer_tag_1_d");
        UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

        PlayersBgImage->SetBrushFromTexture(texture);
    }

    if (Index + 1 < 4)
    {
        FString texturepath = TEXT("/Game/Resources/UI/Lobby/t_ui_multi_kitchen_add_gamer_d.t_ui_multi_kitchen_add_gamer_d");
        UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

        AddPlayersBgImage = FindChildWidget<UImage>("PlayerBgImg_" + FString::FromInt(Index + 1), LobbyCanvasPanel);
        if (AddPlayersBgImage == nullptr) return;

        AddPlayersBgImage->SetBrushFromTexture(texture);
    }

    if (Texture != nullptr)
    {
        UTexture2D* PlayerTexture = Texture;

        PlayersImage->SetBrushFromTexture(PlayerTexture);
        PlayersImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        FString texturepath = TEXT("/Game/Resources/UI/Title/Level_Character_Icon_Buck_FullBody.Level_Character_Icon_Buck_FullBody");
        UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

        PlayersImage->SetBrushFromTexture(texture);
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
