// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleStartWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/PlayerController.h"

void UTitleStartWidget::NativeConstruct()
{
	Super::NativeConstruct();




	Background->SetVisibility(ESlateVisibility::Hidden);
	UEImage->SetVisibility(ESlateVisibility::Hidden);

}

void UTitleStartWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	ImgTimeline.TickTimeline(DeltaTime);

	StartAnimTime += DeltaTime;

	if (StartAnimTime >= 3.0f && bIsGhost == false)
	{
		bIsGhost = true;
		Background->SetVisibility(ESlateVisibility::Visible);
		StartImg->SetVisibility(ESlateVisibility::Hidden);
		GhostImg->SetVisibility(ESlateVisibility::Visible);
		GhostImg->SetRenderOpacity(0.0f);
		CurImg = GhostImg;


		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("StartTitleAnimation"));
		ImgTimeline.AddInterpFloat(ImgCurve, ProgressFunction);

		FOnTimelineEvent FinishEvent;
		FinishEvent.BindUFunction(this, FName("PlayTeam17Animation"));
		ImgTimeline.SetTimelineFinishedFunc(FinishEvent);


		ImgTimeline.SetLooping(false);
		ImgTimeline.SetPlayRate(1.0f);
		ImgTimeline.Stop();
		ImgTimeline.PlayFromStart();

		StartAnimTime = 0.0f;
	}
}


void UTitleStartWidget::StartTitleAnimation(float Value)
{
	float CurOpacity = FMath::Lerp(0.0f, 1.0f, Value);

	UpdateImageOpacity(CurOpacity);
}

void UTitleStartWidget::UpdateImageOpacity(float CurOpacity)
{
	CurImg->SetRenderOpacity(CurOpacity);
}

void UTitleStartWidget::PlayTeam17Animation()
{

	CurImg->SetVisibility(ESlateVisibility::Hidden);
	T17Img->SetVisibility(ESlateVisibility::Visible);

	T17Img->SetRenderOpacity(0.0f);
	CurImg = T17Img;

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("StartTitleAnimation"));
	ImgTimeline.AddInterpFloat(ImgCurve, ProgressFunction);

	FOnTimelineEvent FinishEvent;
	FinishEvent.BindUFunction(this, FName("PlayUEAnimation"));
	ImgTimeline.SetTimelineFinishedFunc(FinishEvent);


	ImgTimeline.SetLooping(false);
	ImgTimeline.SetPlayRate(1.0f);
	ImgTimeline.Stop();
	ImgTimeline.PlayFromStart();
}

void UTitleStartWidget::PlayUEAnimation()
{

	CurImg->SetVisibility(ESlateVisibility::Hidden);
	UEImage->SetVisibility(ESlateVisibility::Visible);

	UEImage->SetRenderOpacity(0.0f);
	CurImg = UEImage;

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("StartTitleAnimation"));
	ImgTimeline.AddInterpFloat(ImgCurve, ProgressFunction);

	FOnTimelineEvent FinishEvent;
	FinishEvent.BindUFunction(this, FName("EndTitleAnimation"));
	ImgTimeline.SetTimelineFinishedFunc(FinishEvent);


	ImgTimeline.SetLooping(false);
	ImgTimeline.SetPlayRate(1.0f);
	ImgTimeline.Stop();
	ImgTimeline.PlayFromStart();
}


void UTitleStartWidget::EndTitleAnimation()
{

	CurImg->SetVisibility(ESlateVisibility::Hidden);
	CurImg = Background;

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("StartTitleAnimation"));
	ImgTimeline.AddInterpFloat(FadeOutCurve, ProgressFunction);

	FOnTimelineEvent FinishEvent;
	FinishEvent.BindUFunction(this, FName("SetVisibleCollapsed"));
	ImgTimeline.SetTimelineFinishedFunc(FinishEvent);

	ImgTimeline.SetLooping(false);
	ImgTimeline.SetPlayRate(1.0f);
	ImgTimeline.Stop();
	ImgTimeline.PlayFromStart();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void UTitleStartWidget::SetVisibleCollapsed()
{

	StartWidget->SetVisibility(ESlateVisibility::Collapsed);
}