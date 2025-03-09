// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Global/OC2Global.h"

void UTitleWidget::StartServer()
{
	UOC2Global::StartServer(GetWorld(), Port, TEXT("LobbyLevel"));
}

void UTitleWidget::Connect()
{
	APlayerController* Controller = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	UOC2Global::ConnectServer(GetWorld(), Controller, IP, Port, LevelName);
}

