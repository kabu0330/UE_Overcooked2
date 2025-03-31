// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingFinalScoreWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"


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

