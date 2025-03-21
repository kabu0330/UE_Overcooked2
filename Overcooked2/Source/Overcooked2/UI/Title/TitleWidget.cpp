// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Global/OC2Global.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "UI/Title/TitleSubmenuWidget.h"



void UTitleWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();



}

void UTitleWidget::StartServer()
{
	UOC2Global::StartServer(GetWorld(), Port, LevelName);
}

void UTitleWidget::Connect()
{
	APlayerController* Controller = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	UOC2Global::ConnectServer(GetWorld(), Controller, IP, Port, LevelName);
}

