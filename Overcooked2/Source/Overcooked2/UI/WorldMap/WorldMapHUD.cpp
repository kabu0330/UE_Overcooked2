// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldMap/WorldMapHUD.h"
#include "UI/WorldMap/UI/WorldMapUserWidget.h"
#include "UI/WorldMap/UI/WorldMapStageWidget.h"
#include "UI/Loading/LoadingWidget.h"
#include "Overcooked2.h"

void AWorldMapHUD::BeginPlay()
{
	Super::BeginPlay();

	WorldMapUserWidget = Cast<UWorldMapUserWidget>(CreateWidget(GetWorld(), WorldWidgetObject));
	LoadingWidget = Cast<ULoadingWidget>(CreateWidget(GetWorld(), LoadingWidgetObject));
	WorldMapStageWidget = Cast<UWorldMapStageWidget>(CreateWidget(GetWorld(), WorldMapStageWidgetObject));

	if (WorldMapUserWidget == nullptr || LoadingWidget == nullptr || WorldMapStageWidget == nullptr)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("¿ùµå¸Ê À§Á¬ È¤Àº ·Îµù À§Á¬ÀÌ nullptr ÀÔ´Ï´Ù!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	}

	WorldMapUserWidget->AddToViewport();
	LoadingWidget->AddToViewport();
	LoadingWidget->SetVisibility(ESlateVisibility::Hidden);

	LoadingWidget->SetIsConnecting(false);
}


void AWorldMapHUD::ShowLoadingWidget()
{
	LoadingWidget->SetVisibility(ESlateVisibility::Visible);
	LoadingWidget->PlayZoomOutAnimation();
}