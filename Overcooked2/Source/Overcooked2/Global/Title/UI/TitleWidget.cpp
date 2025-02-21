// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Title/UI/TitleWidget.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::StartServer()
{
	FString OpenLevel;
	FString LevelPath = TEXT("/Game/Level/PlayLevel");
	OpenLevel = FString::Printf(TEXT(":%s%s"), *Port, *LevelPath);

	UGameplayStatics::OpenLevel(GetWorld(), *OpenLevel, true, TEXT("listen"));
}

void UTitleWidget::Connect()
{
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *IP, *Port);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ConnectLevelName));
}


