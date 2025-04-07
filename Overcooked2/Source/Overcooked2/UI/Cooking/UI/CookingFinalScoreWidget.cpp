// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingFinalScoreWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "UI/Cooking/CaptureComponent2D.h"
#include "Character/OC2Character.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Kismet/GameplayStatics.h"

void UCookingFinalScoreWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    CurPlayerCount = 4;

	for (int i = 0; i < CurPlayerCount; i++)
	{
        ShowPlayers(i);
	}
}


void UCookingFinalScoreWidget::ShowPlayers(int Index)
{
    {

        AOC2Character* PlayerCharacter = nullptr;
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (!PC) continue;

            PlayerCharacter = Cast<AOC2Character>(PC->GetPawn());
            if (PlayerCharacter)
            {
                UE_LOG(LogTemp, Warning, TEXT("플레이어 컨트롤러를 통해 찾은 캐릭터: %s"), *PlayerCharacter->GetName());
            }
        }


        UCaptureComponent2D* CaptureComponent = PlayerCharacter->FindComponentByClass<UCaptureComponent2D>();
        if (!CaptureComponent) return;
        CaptureComponent->TextureTarget = RenderTarget0;
        UTextureRenderTarget2D* RenderedTexture = CaptureComponent->TextureTarget;
        if (!RenderedTexture || !RenderedTexture->IsValidLowLevelFast()) return;


        UMaterialInstanceDynamic* DynamicMaterial = PlayerImg_0->GetDynamicMaterial();
        if (DynamicMaterial)
        {
            DynamicMaterial->SetTextureParameterValue(FName("TextureParam"), RenderedTexture);

            PlayerImg_0->SetBrushFromMaterial(DynamicMaterial);
            PlayerImg_0->SynchronizeProperties();
        }


    }



    FString PlayerNum = "Player_" + FString::FromInt(Index);
    UCanvasPanel* Player = FindChildWidget<UCanvasPanel>(PlayerNum, PlayerCanvas);

    if (Player != nullptr)
    {
        static const TMap<int, TArray<float>> XOffsetsMap = {
            {1, {0.0f}},
            {2, {-1.0f, 1.0f}},
            {3, {-1.0f, 0.0f, 1.0f}},
            {4, {-1.5f, -0.5f, 0.5f, 1.5f}}
        };

        const float SizeX = 350.0f;

        float XOffset = 0.0f;

        if (XOffsetsMap.Contains(CurPlayerCount))
        {
            TArray<float> XOffsetArray = XOffsetsMap[CurPlayerCount]; 

            if (XOffsetArray.IsValidIndex(Index))
            {
                XOffset = XOffsetArray[Index] * SizeX;
            }
        }

        Player->SetRenderTranslation({ XOffset, 0.0f });
        Player->SetVisibility(ESlateVisibility::Visible);
    }
}


template <typename T>
T* UCookingFinalScoreWidget::FindChildWidget(const FString& Name, UCanvasPanel* Canvas)
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

