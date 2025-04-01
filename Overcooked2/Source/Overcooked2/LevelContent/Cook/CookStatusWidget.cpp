// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/CookStatusWidget.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UCookStatusWidget::SetStatusTexture(UTexture2D* NewTexture)
{
    if (StatusImage && NewTexture)
    {
        FSlateBrush NewBrush;
        NewBrush.SetResourceObject(NewTexture);
        NewBrush.ImageSize = FVector2D(NewTexture->GetSizeX(), NewTexture->GetSizeY());

        StatusImage->SetBrush(NewBrush);
    }
}
