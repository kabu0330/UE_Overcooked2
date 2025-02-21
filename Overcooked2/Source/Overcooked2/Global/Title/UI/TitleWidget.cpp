// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Title/UI/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Global/OC2Global.h"

void UTitleWidget::StartServer()
{
	FString OpenLevel;
	FString LevelPath = TEXT("");

	UOC2Global::GetAssetPackageName(UWorld::StaticClass(), TEXT("GlobalDevLevel"), LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *Port, *LevelPath);

	UGameplayStatics::OpenLevel(GetWorld(), *OpenLevel, true, TEXT("listen"));
}

void UTitleWidget::Connect()
{
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *IP, *Port);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ConnectLevelName));
}


