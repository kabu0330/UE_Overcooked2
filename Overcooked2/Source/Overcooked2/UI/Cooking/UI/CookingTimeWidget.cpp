// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingTimeWidget.h"
#include "Overcooked2.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

#include "Sound/SoundBase.h" 
#include "Global/Data/OC2GlobalData.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UCookingTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurTime = TotalTime;

	OriginalFontSize = Time->GetFont().Size;

	UE_LOG(OVERCOOKED_LOG, Log, TEXT("CookingTimeWidget is Ready"));


}

void UCookingTimeWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	TimerImgTimeline.TickTimeline(DeltaTime);
	TimerTextTimeline.TickTimeline(DeltaTime);

}

float UCookingTimeWidget::StartTimerTick(float CurTimeValue)
{
	float curtime = 0.0f;

	if (true == bIsStart && CurTime > 0.0f)
	{
		curtime = StartTimer(CurTimeValue);
	}
	//else if (true == bIsStart && CurTime <= 0.0f)
	//{
	//	//bIsTimesUP = true;
	//	TimerTextTimeline.SetLooping(false);
	//}

	if (CurTime <= 10.f  && TimerImgTimeline.IsPlaying() == false)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("PlayTimerAnimation"));
		TimerImgTimeline.AddInterpFloat(TimerCurve, ProgressFunction);

		TimerImgTimeline.SetLooping(true);
		TimerImgTimeline.SetPlayRate(8.0f);
		TimerImgTimeline.Stop();
		TimerImgTimeline.PlayFromStart();


		FOnTimelineFloat ProgressFunction1;
		ProgressFunction1.BindUFunction(this, FName("PlayTimerTextAnimation"));
		TimerTextTimeline.AddInterpFloat(TimerTextCurve, ProgressFunction1);

		TimerTextTimeline.SetLooping(true);
		TimerTextTimeline.SetPlayRate(1.0f);
		TimerTextTimeline.Stop();
		TimerTextTimeline.PlayFromStart();

	}

	return curtime;

}

void UCookingTimeWidget::PlayTimerTextAnimation(float Value)
{
	if (Value <= 0)
	{
		if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "CookingTimerBeep"))
		{
			if(CurTime > 0.0f)
			{
				UGameplayStatics::PlaySound2D(this, Sound);
			}
		}
	}
	FLinearColor TargetColor(45.f / 255.f, 25.f / 255.f, 15.f / 255.f);
	FLinearColor CurColor = FMath::Lerp(FLinearColor::White, TargetColor, Value);

	FVector2D CurTranslation = FMath::Lerp(FVector2D(0.0f, 0.0f), FVector2D(0.0f, -5.0f), Value);
	float CurSize = FMath::Lerp(OriginalFontSize, OriginalFontSize + 5, Value);

	UpdateTimerScale(CurColor, CurSize, CurTranslation);

}

void UCookingTimeWidget::UpdateTimerScale(FLinearColor Color, float FontSize, FVector2D Position)
{
	if (Time != nullptr)
	{
		Time->SetColorAndOpacity(Color);	

		FSlateFontInfo FontInfo = Time->GetFont();
		FontInfo.Size = FontSize;

		Time->SetFont(FontInfo);
		Time->SetRenderTranslation(Position);
	}
}


void UCookingTimeWidget::PlayTimerAnimation(float Value)
{
	float CurAngle = FMath::Lerp(-2.0f, 2.0f, Value);

	UpdateTimerAngle(CurAngle);

}

void UCookingTimeWidget::UpdateTimerAngle(float Angle)
{
	TimeImg->SetRenderTransformAngle(Angle);

}

void UCookingTimeWidget::SetStartTimer(bool IsStart)
{
	bIsStart = IsStart;
}


float UCookingTimeWidget::StartTimer(float DeltaTime)
{
	float PrevTime = CurTime;

	CurTime = DeltaTime;


	int Min = static_cast<int>(CurTime / 60);
	int Sec = static_cast<int>(CurTime) % 60;

	if (Min <= 0 && Sec <= 0)
	{
		Min = 0;
		Sec = 0;
	}


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

	FLinearColor Color = TimeProgressBar->GetFillColorAndOpacity();

	float TimeLimit = 180.0f;

	if (CurTime > (TimeLimit / 3) * 2)
	{
		TimeProgressBar->SetFillColorAndOpacity({ Color.R, Color.G, 0.0f });
	}	
	else if (CurTime > TimeLimit / 3)
	{
		TimeProgressBar->SetFillColorAndOpacity({ Color.R + (PrevTime - CurTime) * 0.01f, Color.G + (PrevTime - CurTime) * 0.01f, 0.0f });
	}
	else 
	{
		TimeProgressBar->SetFillColorAndOpacity({ Color.R + (PrevTime - CurTime) * 0.01f, Color.G - (PrevTime - CurTime) * 0.02f, 0.0f });
	}




	return CurTime;
}