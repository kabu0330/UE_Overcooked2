// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingReceiptWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Global/OC2Global.h"

#include "Kismet/GameplayStatics.h"

#include "Sound/SoundBase.h" 
#include "Global/Data/OC2GlobalData.h"
#include "Components/AudioComponent.h"

void UCookingReceiptWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("PlayStarAnimaion"));
    StarTimeline.AddInterpFloat(StarCurve, ProgressFunction);

    FOnTimelineEvent FinishEvent;
    FinishEvent.BindUFunction(this, FName("ResetSize"));
    StarTimeline.SetTimelineFinishedFunc(FinishEvent);

}

void UCookingReceiptWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    StarTimeline.TickTimeline(DeltaTime);
}

void UCookingReceiptWidget::CheckStar()
{
    //this;
    //UWorld* World = GetWorld();

    TotalStartScore = UOC2Global::GetTotalScore(GetWorld());

    if (TotalStartScore < FistStartScore)
    {
        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultSound"))
        {
            UGameplayStatics::PlaySound2D(this, Sound, 0.5f);
        }

        return;
    }
    else if (TotalStartScore >= FistStartScore && TotalStartScore < SecondStartScore)
    {
        TotalStar = 1;


    }
    else if (TotalStartScore >= SecondStartScore && TotalStartScore < ThirdStartScore)
    {
        TotalStar = 2;



    }
    else if (TotalStartScore >= ThirdStartScore)
    {
        TotalStar = 3;

    }


    StarTimeline.SetLooping(false);
    StarTimeline.SetPlayRate(1.0f / 0.5f);
    StarTimeline.Stop();
    StarTimeline.PlayFromStart();

}

void UCookingReceiptWidget::ShowScoreText()
{
    GetWorld()->GetTimerManager().ClearTimer(ScoreTextTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(ScoreTextTimerHandle, this, &UCookingReceiptWidget::PlayScoreTextAnimation, 0.3f, true);
}

void UCookingReceiptWidget::PlayScoreTextAnimation()
{
    if (CurTime == 0)
    {
        UTextBlock* DeliveredTxt = FindChildWidget<UTextBlock>("DeliveredTxt", TxtCanvas);
        UTextBlock* DeliveredCount = FindChildWidget<UTextBlock>("DeliveredCount", TxtCanvas);

        DeliveredTxt->SetVisibility(ESlateVisibility::Visible);
        DeliveredCount->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultDes0"))
        {
            UGameplayStatics::PlaySound2D(this, Sound, 0.5f);
        }
    }
    else if (CurTime == 1)
    {
        UTextBlock* DeliveredScore = FindChildWidget<UTextBlock>("DeliveredScore", TxtCanvas);

        FString Score = FString::FromInt(UOC2Global::GetOrderScore(GetWorld()));
        
        DeliveredScore->SetText(FText::FromString(Score));
        DeliveredScore->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultDesScore"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    else if (CurTime == 2)
    {
        UTextBlock* TipsTxt = FindChildWidget<UTextBlock>("TipsTxt", TxtCanvas);
        TipsTxt->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultDes1"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    else if (CurTime == 3)
    {
        UTextBlock* TipScore = FindChildWidget<UTextBlock>("TipScore", TxtCanvas);
        FString Score = FString::FromInt(UOC2Global::GetFeverScore(GetWorld()));

        TipScore->SetText(FText::FromString(Score));
        TipScore->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultDesScore"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    else if (CurTime == 4)
    {
        UTextBlock* FailedTxt = FindChildWidget<UTextBlock>("FailedTxt", TxtCanvas);
        UTextBlock* FailedCount = FindChildWidget<UTextBlock>("FailedCount", TxtCanvas);

        FailedTxt->SetVisibility(ESlateVisibility::Visible);
        FailedCount->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultDes2"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    else if (CurTime == 5)
    {
        UTextBlock* FailedScore = FindChildWidget<UTextBlock>("FailedScore", TxtCanvas);
        FString Score = FString::FromInt(UOC2Global::GetFailScore(GetWorld()));

        FailedScore->SetText(FText::FromString(Score));
        FailedScore->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultDesScore"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    else if (CurTime == 6)
    {
        UTextBlock* TotalTxt = FindChildWidget<UTextBlock>("TotalTxt", TxtCanvas);
        TotalTxt->SetVisibility(ESlateVisibility::Visible);

        UTextBlock* TotalScore = FindChildWidget<UTextBlock>("TotalScore", TxtCanvas);
        FString Score = FString::FromInt(UOC2Global::GetTotalScore(GetWorld()));

        TotalScore->SetText(FText::FromString(Score)); 
        TotalScore->SetVisibility(ESlateVisibility::Visible);

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultFinalScore"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    else
    {


        GetWorld()->GetTimerManager().ClearTimer(ScoreTextTimerHandle);
    }

    CurTime++;
}

void UCookingReceiptWidget::PlayStarAnimaion(float Value)
{
 
    FVector2D CurSize = FMath::Lerp(FVector2D(0.0f, 0.0f), FVector2D(2.0f, 2.0f), Value);

    UpdateStar(CurIndex, CurSize);
}

void UCookingReceiptWidget::UpdateStar(int Count, FVector2D Size)
{
    if (ReceiptCanvas != nullptr)
    {
        UImage* StarImage = FindChildWidget<UImage>("Star_" + FString::FromInt(Count), ReceiptCanvas);
        StarImage->SetVisibility(ESlateVisibility::Visible);
        StarImage->SetRenderScale(Size);
        CurImg = StarImage;
    }
}

void UCookingReceiptWidget::ResetSize()
{
    FString Str = FString::FromInt(CurIndex);
    FName Name = FName("ResultStar" + Str);

    if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), Name))
    {
        UGameplayStatics::PlaySound2D(this, Sound);
    }
    CurIndex += 1;

    if(CurIndex < TotalStar)
    {
        StarTimeline.Stop();
        StarTimeline.PlayFromStart();
    }
    else if (CurIndex == TotalStar)
    {
        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultSound"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }
    }
    CurImg->SetRenderScale(FVector2D(1.0f, 1.0f));


}

template <typename T>
T* UCookingReceiptWidget::FindChildWidget(const FString& Name, UCanvasPanel* Canvas)
{
    if (!Canvas) return nullptr;

    FString TargetPrefix = Name;
    TArray<UWidget*> Children = Canvas->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (T* Widget = Cast<T>(Child))
        {
            if (Widget->GetName().StartsWith(TargetPrefix))
            {
                return Widget;
            }
        }
    }

    return nullptr;
}

