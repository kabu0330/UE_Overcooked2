// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingTimeWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UCookingTimeWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (true == bIsStart)
	{
		StartTimer(DeltaTime);
	}
	else
	{
		CurTime = TotalTime;
	}
}


void UCookingTimeWidget::StartTimer(float Deltatime)
{

	int Min = static_cast<int>(CurTime / 60);
	int Sec = static_cast<int>(CurTime) % 60;

	FString MinStr = "0";
	FString SecStr = "0";

	if (Min < 10)
	{
		MinStr = "0" + FString::FromInt(Min);
	}
	else
	{
		MinStr = FString::FromInt(Min);
	}

	if (Sec < 10)
	{
		SecStr = "0" + FString::FromInt(Sec);
	}
	else
	{
		SecStr = FString::FromInt(Sec);
	}

	Time->SetText(FText::FromString(MinStr + ":" + SecStr));
	TimeProgressBar->SetPercent(CurTime / TotalTime);

	CurTime -= Deltatime;

}