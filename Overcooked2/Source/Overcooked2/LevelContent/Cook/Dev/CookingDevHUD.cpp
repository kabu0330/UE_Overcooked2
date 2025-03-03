// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevHUD.h"
#include <LevelContent/Cook/Dev/CookingDevUserWidget.h>

ACookingDevHUD::ACookingDevHUD()
{
}

ACookingDevHUD::~ACookingDevHUD()
{
}

void ACookingDevHUD::BeginPlay()
{
	Super::BeginPlay();
	if (nullptr != WidgetSubclass)
	{
		Widget = CreateWidget<UCookingDevUserWidget>(GetWorld(), WidgetSubclass);
		if (nullptr != Widget)
		{
			Widget->AddToViewport();
			//Widget->SetKeyboardFocus();
		}
	}

}

void ACookingDevHUD::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}
