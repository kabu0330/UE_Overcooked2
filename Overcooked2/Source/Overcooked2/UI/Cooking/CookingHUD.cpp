// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/CookingHUD.h"
#include "UI/CookingWidget.h"
#include "Overcooked2.h"

void ACookingHUD::BeginPlay()
{
	Super::BeginPlay();

	CookWidget = Cast<UCookingWidget>(CreateWidget(GetWorld(), CookWidgetObject));

	if (CookWidget == nullptr)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("ÄîÀ§Á¬ÀÌ nullptr ÀÔ´Ï´Ù!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	}

	CookWidget->AddToViewport();
}
