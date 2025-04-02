// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Widget/PlateIconWidget.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UPlateIconWidget::Init()
{
	TArray<UImage*> Slots = { ImageSlot1, ImageSlot2, ImageSlot3 };
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetVisibility(ESlateVisibility::Collapsed); // 없는 건 숨김
	}
}

void UPlateIconWidget::SetIngredientTextures(const TArray<UTexture2D*>& InTextures)
{
    TArray<UImage*> Slots = { ImageSlot1, ImageSlot2, ImageSlot3 };

    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (Slots[i])
        {
            if (i < InTextures.Num() && nullptr != InTextures[i])
            {
                Slots[i]->SetBrushFromTexture(InTextures[i]);
                Slots[i]->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                Slots[i]->SetVisibility(ESlateVisibility::Collapsed); // 없는 건 숨김
            }
        }
    }

	if (1 == InTextures.Num())
	{
		Slots[0]->SetRenderTranslation(FVector2D(32.0f, 0.0f));
	}
	else if (2 == InTextures.Num())
	{
		const FVector2D Offset(0.0f, 0.0f); // Y축 아래로

		for (int32 i = 0; i < Slots.Num(); ++i)
		{
			if (Slots[i] && Slots[i]->Visibility == ESlateVisibility::Visible)
			{
				Slots[i]->SetRenderTranslation(FVector2D::ZeroVector);
				Slots[i]->SetRenderTranslation(Offset);
			}
		}
	}
	else
	{
		// 3개 다 있을 경우엔 위치 초기화 (위로 다시 올림)
		for (int32 i = 0; i < Slots.Num(); ++i)
		{
			if (Slots[i])
			{
				Slots[i]->SetRenderTranslation(FVector2D::ZeroVector);
			}
			if (2 == i)
			{
				Slots[i]->SetRenderTranslation(FVector2D(0.0f, 0.0f));
			}
		}
	}

}

void UPlateIconWidget::Reset()
{
    TArray<UImage*> Slots = { ImageSlot1, ImageSlot2, ImageSlot3 };
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        Slots[i]->SetBrushFromTexture(nullptr);
        Slots[i]->SetVisibility(ESlateVisibility::Collapsed);
		Slots[i]->SetRenderTranslation(FVector2D::ZeroVector);
    }
}
