// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleStartWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"

#include "Input/Reply.h"                   // FReply
#include "Input/Events.h"                  // FKeyEvent

#include "Sound/SoundBase.h" 
#include "Kismet/GameplayStatics.h" 
#include "Global/Data/OC2GlobalData.h"


void UTitleStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	this->SetKeyboardFocus();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	CurrentPhase = ETitleAnim::OvercookedFadeIn;
	Background->SetVisibility(ESlateVisibility::Hidden);
	UEImage->SetVisibility(ESlateVisibility::Hidden);

	//USoundBase* TitleSound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSound");
	//UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, TitleSound);
	if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "TitleSound"))
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}


void UTitleStartWidget::SkipAnimation()
{
	if (CurrentPhase == ETitleAnim::OvercookedFadeIn)
	{
		SetGhostTownAnimation();
		return;
	}

	if (ImgTimeline.IsPlaying() == true)
	{
		ImgTimeline.Stop();

		switch (CurrentPhase)
		{
		case ETitleAnim::GhostFadeIn:
			PlayTeam17Animation();
			break;
		case ETitleAnim::Team17FadeIn:
			PlayUEAnimation();	
			break;
		case ETitleAnim::UEFadeIn:
			EndTitleAnimation();
			break;
		default:
			break;
		}
	}

}

FReply UTitleStartWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::SpaceBar)
	{
		SkipAnimation();
		return FReply::Handled();
	}
	else if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		SetVisibleCollapsed();
		SetIsFocusable(true);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}


		return FReply::Handled();
	}
	return FReply::Unhandled();
}




void UTitleStartWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	ImgTimeline.TickTimeline(DeltaTime);

	StartAnimTime += DeltaTime;

	if (StartAnimTime >= 3.0f && bIsGhost == false)
	{

		SetGhostTownAnimation();
		StartAnimTime = 0.0f;
	}
}

void UTitleStartWidget::SetGhostTownAnimation()
{
	bIsGhost = true;

	Background->SetVisibility(ESlateVisibility::Visible);
	StartImg->SetVisibility(ESlateVisibility::Hidden);
	GhostImg->SetVisibility(ESlateVisibility::Visible);
	GhostImg->SetRenderOpacity(0.0f);
	CurImg = GhostImg;
	CurrentPhase = ETitleAnim::GhostFadeIn;

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
	CurrentPhase = ETitleAnim::Team17FadeIn;

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
	CurrentPhase = ETitleAnim::UEFadeIn;

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