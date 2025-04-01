// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Widget/PlateIconWidget.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

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
                Slots[i]->SetVisibility(ESlateVisibility::Collapsed); // ¾ø´Â °Ç ¼û±è
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
    }
}
