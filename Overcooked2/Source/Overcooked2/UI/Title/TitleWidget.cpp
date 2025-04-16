// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Global/OC2Global.h"
#include "Global/Data/OC2GlobalData.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "UI/Title/TitleSubmenuWidget.h"

#include "Sound/SoundBase.h" 
#include "Kismet/GameplayStatics.h" 
#include "Global/Data/OC2GlobalData.h"
#include "Components/AudioComponent.h"


void UTitleWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();



}

void UTitleWidget::PlaySelectSound()
{

	USoundBase* SelectButtonSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "SelectSound");
	UGameplayStatics::PlaySound2D(this, SelectButtonSound, 0.5f);
	
}

void UTitleWidget::PlaySubMenuSound()
{
	USoundBase* SubmenuSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "SubmenuSound");
	UGameplayStatics::PlaySound2D(this, SubmenuSound, 0.5f);
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

void UTitleWidget::PlayOpenSound()
{
	USoundBase* ShutterSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleShutterSound");
	UGameplayStatics::PlaySound2D(this, ShutterSound);

	USoundBase* StartSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleStartSound");
	UGameplayStatics::PlaySound2D(this, StartSound);
}
