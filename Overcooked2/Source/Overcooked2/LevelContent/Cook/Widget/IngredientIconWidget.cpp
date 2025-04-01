// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Widget/IngredientIconWidget.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UIngredientIconWidget::SetTexture(UTexture2D* NewTexture)
{
    if (IconImage && NewTexture)
    {
        FSlateBrush NewBrush;
        NewBrush.SetResourceObject(NewTexture);
        NewBrush.ImageSize = FVector2D(NewTexture->GetSizeX(), NewTexture->GetSizeY());

        IconImage->SetBrush(NewBrush);
    }
}
