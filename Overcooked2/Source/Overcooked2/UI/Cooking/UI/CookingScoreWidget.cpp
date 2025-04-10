// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingScoreWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCookingScoreWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UImage* CoinImg = Cast<UImage>(GetWidgetFromName(TEXT("CoinImage")));
    if (CoinImg != nullptr)
    {
        CoinMaterial = CoinImg->GetDynamicMaterial();

        if (!CoinMaterial)
        {
            CoinMaterial = CoinImg->GetDynamicMaterial();
        }
    }

    if (CoinFlameImage != nullptr)
    {
        CoinFlameMaterial = CoinFlameImage->GetDynamicMaterial();

        if (!CoinFlameMaterial)
        {
            CoinFlameMaterial = CoinFlameImage->GetDynamicMaterial();
        }
    }

    OriginalFontSize = ScoreText->GetFont().Size;


    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("PlayScoreAnimation"));
    ScoreTimeline.AddInterpFloat(ScoreCurve, ProgressFunction);

    FOnTimelineEvent FinishEvent;
    FinishEvent.BindUFunction(this, FName("ResetScaleAndColor"));
    ScoreTimeline.SetTimelineFinishedFunc(FinishEvent);

    FOnTimelineFloat ProgressFunction1;
    ProgressFunction1.BindUFunction(this, FName("PlayAddScoreAnimation"));
    AddScoreTimeline.AddInterpFloat(AddScoreCurve, ProgressFunction1);

    FOnTimelineEvent FinishEvent1;
    FinishEvent1.BindUFunction(this, FName("ResetLocation"));
    AddScoreTimeline.SetTimelineFinishedFunc(FinishEvent1);

    AddText->SetVisibility(ESlateVisibility::Hidden);
}


void UCookingScoreWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    ScoreTimeline.TickTimeline(DeltaTime);
    AddScoreTimeline.TickTimeline(DeltaTime);
    
}


void UCookingScoreWidget::SetTipCount(int TipCount)
{

    FString TexturePath = TEXT("/Game/Resources/UI/Cooking/Coin/coin_banner_bar.coin_banner_bar");
    FString Tip = TEXT("");
    if (TipCount == 1)
    {
        TexturePath = TEXT("/Game/Resources/UI/Cooking/Coin/coin_banner_bar.coin_banner_bar");

        TipText->SetVisibility(ESlateVisibility::Visible);
        Tip = TEXT("TIP X 1");

    }
    else if (TipCount == 2)
    {
        TexturePath = TEXT("/Game/Resources/UI/Cooking/Coin/coin_banner_bar_01.coin_banner_bar_01");

        TipText->SetVisibility(ESlateVisibility::Visible);
        Tip = TEXT("TIP X 2");

    }
    else if (TipCount == 3)
    {
        TexturePath = TEXT("/Game/Resources/UI/Cooking/Coin/coin_banner_bar_02.coin_banner_bar_02");

        TipText->SetVisibility(ESlateVisibility::Visible);
        Tip = TEXT("TIP X 3");

    }
    else if (TipCount == 4)
    {
        TexturePath = TEXT("/Game/Resources/UI/Cooking/Coin/coin_banner_bar_03.coin_banner_bar_03");

        TipText->SetVisibility(ESlateVisibility::Visible);
        Tip = TEXT("TIP X 4");

    }

    class UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));
    CoinBannerBar->SetBrushFromTexture(Texture);
    TipText->SetText(FText::FromString(Tip));
}


void UCookingScoreWidget::PlayCoinFlameAnimation()
{
    if (!CoinFlameMaterial) return;
    CoinFlameImage->SetVisibility(ESlateVisibility::Visible);

    float AnimationDuration = 0.58f;
    float TimeStep = 0.02f;
    float CurrentTime = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(FlameAnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(FlameAnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime >= AnimationDuration)
            {
                CurrentTime = 0.0f;
            }

            CoinFlameMaterial->SetScalarParameterValue(TEXT("CoinFlameTime"), CurrentTime);
            CurrentTime += 0.01;

        }, TimeStep, true);
}

void UCookingScoreWidget::StopCoinFlameAnimation()
{
    CoinFlameImage->SetVisibility(ESlateVisibility::Hidden);
    GetWorld()->GetTimerManager().ClearTimer(FlameAnimationTimer);
}

void UCookingScoreWidget::PlayOderCompleteAnimation(int Score)
{
    PlayCoinAnimation();

    ScoreTimeline.SetLooping(false);
    ScoreTimeline.SetPlayRate(1.0f / 0.5f);
    ScoreTimeline.Stop();
    ScoreTimeline.PlayFromStart();

    TotalScore += Score;

    FString AddScore = "+" + FString::FromInt(Score);

    AddText->SetText(FText::FromString(AddScore));
    AddText->SetVisibility(ESlateVisibility::Visible);

    AddScoreTimeline.SetLooping(false);
    AddScoreTimeline.SetPlayRate(1.0f / 0.5f);
    AddScoreTimeline.Stop();
    AddScoreTimeline.PlayFromStart();
}

void UCookingScoreWidget::PlayCoinAnimation()
{
    if (!CoinMaterial) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(AnimationTimer)) return;

    float AnimationDuration = 1.0f;
    float TimeStep = 0.01f;
    float CurrentTime = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);

    GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this, AnimationDuration, TimeStep, CurrentTime]() mutable
        {
            if (CurrentTime >= AnimationDuration)
            {
                GetWorld()->GetTimerManager().ClearTimer(AnimationTimer);
                return;
            }

            CoinMaterial->SetScalarParameterValue(TEXT("CoinTime"), CurrentTime);
            CurrentTime += TimeStep;

        }, TimeStep, true);
}

void UCookingScoreWidget::PlayAddScoreAnimation(float Value)
{

    float CurOpacity = FMath::Lerp(1.0, 0.0, Value);
    float CurYPos = FMath::Lerp(0.0, -30.0, Value);

    AddText->SetRenderOpacity(CurOpacity);
    AddText->SetRenderTranslation({0.0f, CurYPos});

}

void UCookingScoreWidget::PlayScoreAnimation(float Value)
{

    FLinearColor CurColor = FMath::Lerp(FLinearColor::White, FLinearColor::Green, Value);
    FVector2D CurTranslation = FMath::Lerp(FVector2D(0.0f, 0.0f), FVector2D(0.0f, -30.0f), Value);
    int CurSize = FMath::Lerp(OriginalFontSize, 80, Value);

    UpdateScore(CurColor, CurSize, CurTranslation);
 
}

void UCookingScoreWidget::UpdateScore(FLinearColor Color, int Size, FVector2D Position)
{
    if (ScoreText != nullptr)
    {
        ScoreText->SetColorAndOpacity(Color);

        FSlateFontInfo FontInfo = ScoreText->GetFont();
        FontInfo.Size = Size;
        FString Total = FString::FromInt(TotalScore);

        ScoreText->SetText(FText::FromString(Total));
        ScoreText->SetFont(FontInfo);
        ScoreText->SetRenderTranslation(Position);
    }
}

void UCookingScoreWidget::ResetScaleAndColor()
{
    ScoreTimeline.ReverseFromEnd();
}

void UCookingScoreWidget::ResetLocation()
{
    AddText->SetVisibility(ESlateVisibility::Hidden);
    AddText->SetRenderOpacity(1.0f);
    AddText->SetRenderTranslation({ 0.0f, 0.0f });
}
