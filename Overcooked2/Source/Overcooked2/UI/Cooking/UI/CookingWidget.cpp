// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/AudioComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Global/Data/OrderDataTable.h"
#include "Global/Data/OC2GlobalData.h"
#include "UI/Cooking/UI/CookingScoreWidget.h"
#include "UI/Cooking/UI/CookingFinalScoreWidget.h"
#include "UI/Cooking/UI/CookingTimeWidget.h"
#include "UI/Cooking/UI/CookingReceiptWidget.h"
#include "UI/Cooking/UI/CookingReadyWidget.h"
#include "UI/Cooking/ReceiptWidgetActor.h"

#include "Overcooked2.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h" 
#include "Containers/Queue.h"



void UCookingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Orders.SetNum(5);
    OrderTime.SetNum(5);

    Orders = { Order_0, Order_1, Order_2, Order_3, Order_4 };

    // Test Setting
    CurOrderCount = 0;

    float StartPos = 0.0f;

    for (int i = 0; i < Orders.Num(); i++)
    {
        Orders[i]->SetRenderTranslation({ StartPos + ImageOffset * (i + 1), 0.0f });

        StartPos += ImageSize;
    }

    for (int i = CurOrderCount; i < Orders.Num(); i++)
    {   
        Orders[i]->SetVisibility(ESlateVisibility::Collapsed);
    }

    {
        CookingScoreWidget = Cast<UCookingScoreWidget>(CreateWidget(GetWorld(), ScoreSubWidget));
        CookingFinalScoreWidget = Cast<UCookingFinalScoreWidget>(CreateWidget(GetWorld(), FinalScoreSubWidget));
        CookingTimerWidget = Cast<UCookingTimeWidget>(CreateWidget(GetWorld(), TimeSubWidget));
        CookingReadyWidget = Cast<UCookingReadyWidget>(CreateWidget(GetWorld(), ReadySubWidget));
        //CookingReceiptWidget = Cast<UCookingReceiptWidget>(CreateWidget(GetWorld(), ReceiptWidget));


        if (CookingScoreWidget != nullptr && CookingTimerWidget != nullptr && CookingFinalScoreWidget != nullptr && CookingReadyWidget != nullptr )
        {
            CookingFinalScoreWidget->SetVisibility(ESlateVisibility::Hidden);
            CookingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
            CookingTimerWidget->SetVisibility(ESlateVisibility::Hidden);
            //SetVisibility(ESlateVisibility::Hidden);

            CookingScoreWidget->AddToViewport();
            CookingTimerWidget->AddToViewport();
            CookingFinalScoreWidget->AddToViewport();
            CookingReadyWidget->AddToViewport();
            //CookingReceiptWidget->AddToViewport();

            //CookingReceiptWidget->SetVisibility(ESlateVisibility::Hidden);

        }
        else
        {
            //UE_LOG(OVERCOOKED_LOG, Error, TEXT("쿡위젯이 nullptr 입니다!!!"));

        }
    }

}

void UCookingWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);
    WrongOrderTimeline.TickTimeline(DeltaTime);
    CompleteOrderTimeline.TickTimeline(DeltaTime);
    for (int i = 0; i < CurOrderCount; i++)
    {
        UpdateOrderTime(i, DeltaTime);
    }

    //if (CookingTimerWidget->GetIsTimesUP() == true)
    //{
    //    ShowTimesUPAnim();
    //    CookingTimerWidget->SetIsTimesUP(false);
    //    CookingTimerWidget->SetStartTimer(false);
    //}

}

float UCookingWidget::StartTimerTick(float DeltaTime)
{
    return CookingTimerWidget->StartTimerTick(DeltaTime);
}

void UCookingWidget::StartTimer()
{
    CookingTimerWidget->SetStartTimer(true);
}

void UCookingWidget::PlayZoomOutAnimation()
{
    CookingReadyWidget->PlayZoomOutAnimation();
}



void UCookingWidget::SetHoldProgress(int Progress)
{
    CookingReadyWidget->SetHoldProgress(Progress);
}

bool UCookingWidget::GetIsReady()
{
    return CookingReadyWidget->GetReady();
}

void UCookingWidget::SetIsReady(bool IsReady)
{
    return CookingReadyWidget->SetReady(IsReady);
}


void UCookingWidget::ShowHoldCanvas()
{
    CookingReadyWidget->HoldCanvas->SetVisibility(ESlateVisibility::Visible);
}


bool UCookingWidget::IsTimesUp()
{
    return CookingTimerWidget->GetIsTimesUP();
}

bool UCookingWidget::IsShowScoreWidget()
{
    return bShowScoreWidget;
}

void UCookingWidget::StartGame()
{
    CookingScoreWidget->SetVisibility(ESlateVisibility::Visible);
    CookingTimerWidget->SetVisibility(ESlateVisibility::Visible);
    CookingReadyWidget->SetVisibility(ESlateVisibility::Hidden);
    SetVisibility(ESlateVisibility::Visible);

    ShowReadyImageAnim();

    CookingReadyWidget->SetReady(false);
}

void UCookingWidget::ShowReadyImageAnim()
{
    ReadyCanvas->SetRenderScale({ 0.0f, 0.0f });
    ReadyCanvas->SetVisibility(ESlateVisibility::Visible);

    if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "CookingReady"))
    {
        UGameplayStatics::PlaySound2D(this, Sound);
    }

    GetWorld()->GetTimerManager().SetTimer(ReadyTimerHandle, this, &UCookingWidget::PlayReadyImageAnim, 0.01f, true);
}


void UCookingWidget::PlayReadyImageAnim()
{
    if (ReadyTimeElapsed >= 3.0f)
    {
        GoCanvas->SetVisibility(ESlateVisibility::Collapsed);
        StartTimer();



        GetWorld()->GetTimerManager().ClearTimer(ReadyTimerHandle);
        return;

    }

    if (ReadyCanvas->GetRenderTransform().Scale.X < 1.0f && ReadyTimeElapsed < 2.0f)
    {
        ReadyCanvas->SetRenderScale({ ReadyCanvas->GetRenderTransform().Scale.X + ReadyOffset, ReadyCanvas->GetRenderTransform().Scale.Y + ReadyOffset });
    }
    else if (ReadyCanvas->GetRenderTransform().Scale.X >= 1.0f && ReadyTimeElapsed <= 2.1f && ReadyTimeElapsed >= 2.0f)
    {
        ReadyCanvas->SetVisibility(ESlateVisibility::Collapsed);
        ReadyOffset = 0.0f;
        GoCanvas->SetRenderScale({ 0.0f, 0.0f });

        if (GoCanvas->GetVisibility() != ESlateVisibility::Visible)
        {
            if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "CookingGo"))
            {
                UGameplayStatics::PlaySound2D(this, Sound);
            }
        }
        GoCanvas->SetVisibility(ESlateVisibility::Visible);

    }
    else if (GoCanvas->GetRenderTransform().Scale.X <= 1.0f && ReadyTimeElapsed < 3.0f)
    {
        GoCanvas->SetRenderScale({ GoCanvas->GetRenderTransform().Scale.X + ReadyOffset, GoCanvas->GetRenderTransform().Scale.Y + ReadyOffset });
    }

    ReadyTimeElapsed += 0.01;
    ReadyOffset += 0.01;
}

void UCookingWidget::ShowTimesUPAnim()
{
    TimesUpCanvas->SetRenderScale({ 0.0f, 0.0f });
    TimesUpCanvas->SetVisibility(ESlateVisibility::Visible);

    if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "CookingTimesUp"))
    {
        UGameplayStatics::PlaySound2D(this, Sound);
    }


    GetWorld()->GetTimerManager().SetTimer(TimesUPTimerHandle, this, &UCookingWidget::PlayTimesUPAnim, 0.01f, true);
}


void UCookingWidget::PlayTimesUPAnim()
{
    if (TimesUpTimeElapsed >= 2.0f)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimesUPTimerHandle);
        return;
    }
    if (nullptr != TimesUpCanvas)
    {
        if (TimesUpCanvas->GetRenderTransform().Scale.X < 1.0f && TimesUpTimeElapsed < 2.0f)
        {
            TimesUpCanvas->SetRenderScale({ TimesUpCanvas->GetRenderTransform().Scale.X + TimesUpOffset, TimesUpCanvas->GetRenderTransform().Scale.Y + TimesUpOffset });
        }
    }

    TimesUpTimeElapsed += 0.01;
    TimesUpOffset += 0.01;
}


void UCookingWidget::ShowReceiptWidget()
{

    for (int i = 0; i < Orders.Num(); i++)
    {
        Orders[i]->SetVisibility(ESlateVisibility::Hidden);
    }
    bShowScoreWidget = true;
    TimesUpCanvas->SetVisibility(ESlateVisibility::Collapsed);
    CookingFinalScoreWidget->SetVisibility(ESlateVisibility::Visible);

    CookingFinalScoreWidget->ShowTexturePlayers();
    //CookingFinalScoreWidget->ShowCapturePlayers();

    if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "ResultVictory"))
    {
        UGameplayStatics::PlaySound2D(this, Sound);
    }


    GetWorld()->GetTimerManager().SetTimer(DelayedScoreHandle, this, &UCookingWidget::PlayScoreAnim, 2.0f, false);
}

void UCookingWidget::PlayScoreAnim()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReceiptWidgetActor::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        AReceiptWidgetActor* TargetActor = Cast<AReceiptWidgetActor>(FoundActors[0]);
        if (nullptr != TargetActor)
        {
            TargetActor->ShowTotalScoreAnim();
        }
    }
}


void UCookingWidget::OrderComplete(int Index, int Score)
{
    if (!ColorCurve) return;
    if (Index >= Orders.Num()) return;


    // if (GetWorld()->GetTimerManager().IsTimerActive(OpacityTimerHandle)) return;


    // 애니메이션 중이면 큐에 저장
    if (true == bAnimatingCompleteOrder)
    {
        CompleteOrders.Enqueue({ Index, Score });
        return;
    }

    bAnimatingCompleteOrder = true;

    // 주문 처리 본체 함수 분리
    ShowOrderComplete(Index, Score);
}

void UCookingWidget::ShowOrderComplete(int Index, int Score)
{

    CompleteOrderNum = Index;
    if (Orders[CompleteOrderNum] == nullptr) return;

    if (Orders[CompleteOrderNum]->GetVisibility() != ESlateVisibility::Collapsed)
    {
        UCanvasPanel* Panel = FindChildPanel("IngredientPanel_", Orders[CompleteOrderNum]);
        UCanvasPanel* ImgPanel = FindChildPanel("I_Img_", Panel);
        UCanvasPanel* IBackPanel = FindChildPanel("IBackImg_", Panel);

        if (ImgPanel == nullptr || Panel == nullptr || IBackPanel == nullptr) return;

        for (int i = 0; i < 3; i++)
        {
            UImage* ToolImg = FindChildImage("Tool" + FString::FromInt(i), ImgPanel);
            if (ToolImg != nullptr)
            {
                ToolImg->SetVisibility(ESlateVisibility::Hidden);
            }
        }


        UImage* BImg = FindChildImage("BIngredient1", IBackPanel);
        UCanvasPanelSlot* BackSlot = Cast<UCanvasPanelSlot>(BImg->Slot);
        BackSlot->SetSize(IShortSize);

        Panel->SetRenderTranslation({ 0.0f, -IngredientArrivePos });

        {

            FOnTimelineFloat ProgressFunction;
            ProgressFunction.BindUFunction(this, FName("UpdateCompleteOrderColor"));
            CompleteOrderTimeline.AddInterpFloat(ColorCurve, ProgressFunction);

            CompleteOrderTimeline.SetLooping(false);
            CompleteOrderTimeline.SetPlayRate(1.0f / 0.5f);

            CompleteOrderTimeline.Stop();
            CompleteOrderTimeline.PlayFromStart();
        }


        if (CookingScoreWidget != nullptr)
        {
            CookingScoreWidget->PlayOderCompleteAnimation(Score);
        }

        if (USoundBase* Sound = UOC2GlobalData::GetUIBaseSound(GetWorld(), "CookingOrderSuccess"))
        {
            UGameplayStatics::PlaySound2D(this, Sound);
        }

        GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
    }
}

void UCookingWidget::CheckFeverTime(int TipCount)
{
    if (TipCount == 4)
    {
        CookingScoreWidget->PlayCoinFlameAnimation();
    }
    else
    {
        CookingScoreWidget->StopCoinFlameAnimation();
    }

    CookingScoreWidget->SetTipCount(TipCount);
}

void UCookingWidget::CreateNewOrder(FOrder Order)
{
    if (CurOrderCount >= Orders.Num())
        return;

    // 애니메이션 중이면 큐에 저장
    if (true == bAnimatingNewOrder || true == bAnimatingCompleteOrder)
    {
        NewOrders.Enqueue(Order);
        return;
    }

    bAnimatingNewOrder = true;

    // 주문 처리 본체 함수 분리
    ShowOrder(Order);
}


void UCookingWidget::ShowOrder(FOrder Order)
{
    NewOrderNum = CurOrderCount;

    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);

    Orders[NewOrderNum]->SetVisibility(ESlateVisibility::Visible);
    Orders[NewOrderNum]->SetRenderTransformAngle(-20.0f);
    Orders[NewOrderNum]->SetRenderOpacity(1.0f);
    Orders[NewOrderNum]->SetRenderTranslation({ ScreenSize.X * 1.5f, 0.0f });
    Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.2f });
    OrderTime[NewOrderNum] = TimeLimit;

    UProgressBar* TimeImg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);
    TimeImg->SetFillColorAndOpacity({ 0.1f, 0.3f, 0.0f });

    CurOrderCount += 1;

    UImage* DishImage = FindChildImage("Dish_", Orders[NewOrderNum]);
    if (DishImage == nullptr) return;

    if (Order.OrderTexutre != nullptr)
    {
        DishImage->SetBrushFromTexture(Order.OrderTexutre);
    }
    else
    {
        FString TexturePath = TEXT("/Game/Resources/UI/Order/Dish/ui_cheeseburger.ui_cheeseburger");
        UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));
        DishImage->SetBrushFromTexture(Texture);
    }

    SettingIngredientImages(Order);

    GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
}

void UCookingWidget::FindOrderImgRecursive(UWidget* Widget, const FLinearColor& _TargetColor)
{
    if (!Widget) return;

    if (UImage* ImageWidget = Cast<UImage>(Widget))
    {

        UpdateImgColor(ImageWidget, _TargetColor);
        return;
    }
    else if (UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(Widget))
    {
        TArray<UWidget*> Children = CanvasPanel->GetAllChildren();
        for (UWidget* Child : Children)
        {
            FindOrderImgRecursive(Child, _TargetColor);
        }
    }
}

void UCookingWidget::UpdateImgColor(UImage* Image, const FLinearColor& Color)
{
    if (!Image) return;

    Image->SetColorAndOpacity(Color);
}

//void UCookingWidget::SubmitWrongOrder()
void UCookingWidget::WrongOrder()
{
    if (!ColorCurve) return;

    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("UpdateAllOrderColor"));
    WrongOrderTimeline.AddInterpFloat(ColorCurve, ProgressFunction);

    FOnTimelineEvent FinishEvent;
    FinishEvent.BindUFunction(this, FName("ResetColor"));
    WrongOrderTimeline.SetTimelineFinishedFunc(FinishEvent);

    WrongOrderTimeline.SetLooping(false);
    WrongOrderTimeline.SetPlayRate(1.0f / 0.5f);

    WrongOrderTimeline.Stop();


    WrongOrderTimeline.PlayFromStart();
}


void UCookingWidget::UpdateAllOrderColor(float Value)
{
    TargetColor = FLinearColor::Red;
    FLinearColor CurrentColor = FMath::Lerp(OriginalColor, TargetColor, Value);

    for (UCanvasPanel* Order : Orders)
    {
        if (!Order) continue;
        FindOrderImgRecursive(Order, CurrentColor);
    }
}

void UCookingWidget::ResetColor()
{
    WrongOrderTimeline.Reverse();
}


void UCookingWidget::UpdateCompleteOrderColor(float Value)
{
    TargetColor = FLinearColor::Green;
    FLinearColor CurrentColor = FMath::Lerp(OriginalColor, TargetColor, Value);

    FindOrderImgRecursive(Orders[CompleteOrderNum], CurrentColor);

}


void UCookingWidget::MoveNewOrder()
{
    if (Orders[NewOrderNum] == nullptr) return;

    FVector2D CurPos = Orders[NewOrderNum]->GetRenderTransform().Translation;
    float CurAngle = Orders[NewOrderNum]->GetRenderTransform().Angle;

    if (NewOrderNum != 0)
    {
        ArrivePos = ImageOffset;
        for (int i = 0; i < CurOrderCount - 1; i++)
        {
            UImage* Img = FindChildImage("OrderBackground_", Orders[i]);
            float scale = Img->GetRenderTransform().Scale.X;
            ArrivePos += ImageSize * scale + ImageOffset;
        }
    }
    else
    {
        ArrivePos = ImageOffset;
    }

    if (CurPos.X <= ArrivePos && CurAngle >= 0.0f)
    {
        Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.0f });
        Orders[NewOrderNum]->SetRenderTranslation({ ArrivePos, 0.0f });
        Orders[NewOrderNum]->SetRenderTransformAngle(0.0f);

        MoveTimeElapsed = 0.0f;
        IngredientTimeElapsed = 0.0f;

        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        bAnimatingNewOrder = false;

        // 다음 재료 이미지 이동 처리 시작
        if (!GetWorld()->GetTimerManager().IsTimerActive(IngredientTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(IngredientTimerHandle, this, &UCookingWidget::UpdateIngredientImagePosition, 0.01f, true);
        }

        // 다음 주문이 큐에 있으면 꺼내서 바로 처리
        FOrder NextOrder;
        if (true == NewOrders.Dequeue(NextOrder))
        {
            CreateNewOrder(NextOrder);
        }

        return;
    }
    else
    {
        // 움직이는 중
        if (CurPos.X > ArrivePos)
        {
            Orders[NewOrderNum]->SetRenderTranslation(CurPos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));
        }

        if (CurAngle < 0.0f)
        {
            Orders[NewOrderNum]->SetRenderTransformAngle(CurAngle + 1.f);
        }
    }

    MoveTimeElapsed += 0.1f;
}


void UCookingWidget::SettingIngredientImages(FOrder& Order)
{
    if (Order.RequireIngredients.IsEmpty())
    {
        return;
    }

    UCanvasPanel* Panel = FindChildPanel("IngredientPanel_", Orders[NewOrderNum]);
    if (Panel == nullptr) return;

    UCanvasPanel* IBackPanel = FindChildPanel("IBackImg_", Panel);
    if (IBackPanel == nullptr) return;

    UCanvasPanel* ImgPanel = FindChildPanel("I_Img_", Panel);
    if (ImgPanel == nullptr) return;

    for (int i = 0; i < Order.RequireIngredients.Num(); i++)
    {
        TArray<UWidget*> BackPanelChind = IBackPanel->GetAllChildren();
        UWidget* IBackImg = BackPanelChind[i];


        if (IBackImg != nullptr)
        {
            UCanvasPanelSlot* BackSlot = Cast<UCanvasPanelSlot>(IBackImg->Slot);
            UImage* BImg = FindChildImage("BIngredient" + FString::FromInt(i), IBackPanel);
            UImage* Img = FindChildImage("IngredientImg" + FString::FromInt(i), ImgPanel);
            Img->SetVisibility(ESlateVisibility::Visible);
            BImg->SetVisibility(ESlateVisibility::Visible);

            UImage* DishImg = FindChildImage("Dish_", Orders[NewOrderNum]);
            DishImg->SetRenderTranslation({ 0.0f, 0.0f });

            UImage* BackImg = FindChildImage("OrderBackground_", Orders[NewOrderNum]);
            BackImg->SetRenderScale({ 1.0f, 1.0f });
            BackImg->SetRenderTranslation({ 0.f, 0.0f });

            UProgressBar* TimeImg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);
            TimeImg->SetRenderScale({ 1.0f, 1.0f });
            TimeImg->SetRenderTranslation({ 0.f, 0.0f });

            if (Order.RequireIngredients[i].IngredientState == EIngredientState::EIS_BOILED)
            {
                UImage* Toolimg = FindChildImage("Tool" + FString::FromInt(i), ImgPanel);
                Toolimg->SetVisibility(ESlateVisibility::Visible);
                BackSlot->SetSize(ILongSize);
            }
            else
            {
                UImage* Toolimg = FindChildImage("Tool" + FString::FromInt(i), ImgPanel);
                Toolimg->SetVisibility(ESlateVisibility::Hidden);
            }


            if (Order.RequireIngredients[i].IngredientTexture != nullptr)
            {
                Img->SetBrushFromTexture(Order.RequireIngredients[i].IngredientTexture);
            }
            else
            {
                FString texturepath = TEXT("/Game/Resources/UI/Order/Ingredient/Fish_Icon.Fish_Icon");
                class UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

                Img->SetBrushFromTexture(texture);
            }
        }
    }

    if (Order.RequireIngredients.Num() < 3)
    {
        for (int i = Order.RequireIngredients.Num(); i < 3; i++)
        {
            UImage* Img = FindChildImage("IngredientImg" + FString::FromInt(i), ImgPanel);
            UImage* BImg = FindChildImage("BIngredient" + FString::FromInt(i), IBackPanel);
            Img->SetVisibility(ESlateVisibility::Hidden);
            BImg->SetVisibility(ESlateVisibility::Hidden);


            UImage* Toolimg = FindChildImage("Tool" + FString::FromInt(i), ImgPanel);
            if (Toolimg != nullptr)
            {
                Toolimg->SetVisibility(ESlateVisibility::Hidden);
            }

        }


        UImage* DishImg = FindChildImage("Dish_", Orders[NewOrderNum]);
        UImage* BackImg = FindChildImage("OrderBackground_", Orders[NewOrderNum]);
        UProgressBar* TimeImg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);

        DishImg->SetRenderTranslation({ -30.0f, 0.0f });

        BackImg->SetRenderScale({ 0.7f, 1.0f });
        BackImg->SetRenderTranslation({ -30.f, 0.0f });

        TimeImg->SetRenderScale({ 0.7f, 1.0f });
        TimeImg->SetRenderTranslation({ -30.f, 0.0f });
    }

}


void UCookingWidget::UpdateOrderTime(int Index, float DeltaTime)
{
    OrderTime[Index] -= DeltaTime;

    UProgressBar* TimeImg = FindChildWidget<UProgressBar>("Time_", Orders[Index]);

    TimeImg->SetPercent({ OrderTime[Index] / TimeLimit });
    FLinearColor Color = TimeImg->GetFillColorAndOpacity();


    float Offset = 30.0f / TimeLimit;

    if (OrderTime[Index] > (TimeLimit / 3) * 2)
    {
        TimeImg->SetFillColorAndOpacity({ Color.R + DeltaTime * 0.7f * Offset, Color.G + DeltaTime * 0.7f * Offset, 0.0f });
    }
    else if (OrderTime[Index] > TimeLimit / 3)
    {
        TimeImg->SetFillColorAndOpacity({ Color.R + DeltaTime * 0.7f * Offset, Color.G - DeltaTime * 0.5f * Offset, 0.0f });
    }
    else
    {
        TimeImg->SetFillColorAndOpacity({ Color.R + DeltaTime * 0.2f * Offset, Color.G - DeltaTime * 0.1f * Offset, 0.0f });
    }

}


void UCookingWidget::UpdateIngredientImagePosition()
{
    if (Orders[NewOrderNum] == nullptr) return;

    UCanvasPanel* Panel = FindChildPanel("IngredientPanel_", Orders[NewOrderNum]);

    if (Panel == nullptr) return;

    FVector2D CurPos = Panel->GetRenderTransform().Translation;

    if (CurPos.Y >= IngredientArrivePos)
    {
        Panel->SetRenderTranslation({ 0.0f, IngredientArrivePos });

        IngredientTimeElapsed = 0.0f;

        GetWorld()->GetTimerManager().ClearTimer(IngredientTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;

    }
    else
    {
        Panel->SetRenderTranslation(CurPos + FVector2D(0.0f, IngredientTargetOffset.Y + IngredientTimeElapsed));
    }


    IngredientTimeElapsed += 0.1f;
}


void UCookingWidget::UpdateImageOpacity()
{

    if (Orders[CompleteOrderNum]->GetRenderOpacity() <= 0.0f && GetWorld()->GetTimerManager().IsTimerActive(MoveTimerHandle) == false)
    {
        Orders[CompleteOrderNum]->SetVisibility(ESlateVisibility::Collapsed);
        FindOrderImgRecursive(Orders[CompleteOrderNum], OriginalColor);
        CompleteOrderTimeline.Stop();
        CurOrderCount -= 1;
        UpdateImagePosition();

        bAnimatingCompleteOrder = false;

        FCompleteOrderData CompleteOrder;
        if (true == CompleteOrders.Dequeue(CompleteOrder))
        {
            OrderComplete(CompleteOrder.Index, CompleteOrder.Score);
        }

        FOrder NextOrder;
        if (true == NewOrders.Dequeue(NextOrder))
        {
            CreateNewOrder(NextOrder);
        }


        GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
        return;
    }

    float CurOpacity = Orders[CompleteOrderNum]->GetRenderOpacity();
    Orders[CompleteOrderNum]->SetRenderOpacity(FMath::Clamp(CurOpacity - OpacityOffset, 0.0f, 1.0f));


    TArray<UWidget*> Children = Orders[CompleteOrderNum]->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (UImage* Image = Cast<UImage>(Child))
        {
            const FLinearColor& Color = Image->GetColorAndOpacity();
            Image->SetColorAndOpacity({ Color.R, Color.G + 1.0f, Color.B, Color.A });
        }
    }

}



void UCookingWidget::UpdateImagePosition()
{

    if (CompleteOrderNum + 1 == Orders.Num())
    {
        return;
    }

    class UCanvasPanel* Panel = Orders[CompleteOrderNum];

    for (int i = CompleteOrderNum; i < CurOrderCount; i++)
    {

        Orders[i] = Orders[i + 1];
        OrderTime[i] = OrderTime[i + 1];
        FinalPos = ImageOffset;

        for (int j = 0; j < i; j++)
        {
            UImage* Img = FindChildImage("OrderBackground_", Orders[j]);
            float Scale = Img->GetRenderTransform().Scale.X;

            FinalPos += ImageSize * Scale + ImageOffset;
        }

        Orders[i]->SetRenderTranslation({ FinalPos, 0.0f });
    }

    Orders[CurOrderCount] = Panel;

    return;

}

template <typename T>
T* UCookingWidget::FindChildWidget(const FString& Name, UCanvasPanel* Canvas)
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


UImage* UCookingWidget::FindChildImage(const FString& Name, UCanvasPanel* Canvas)
{
    FString TargetPrefix = Name;

    TArray<UWidget*> Children;
    Children = Canvas->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (UImage* Image = Cast<UImage>(Child))
        {
            FString ImgName = Image->GetName();
            if (ImgName.StartsWith(TargetPrefix))
            {
                return Image;
            }
        }
    }

    return nullptr;
}

UCanvasPanel* UCookingWidget::FindChildPanel(const FString& Name, UCanvasPanel* Canvase)
{
    FString TargetPrefix = Name;

    TArray<UWidget*> Children;
    Children = Canvase->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (UCanvasPanel* panel = Cast<UCanvasPanel>(Child))
        {
            FString panelname = panel->GetName();
            if (panelname.StartsWith(TargetPrefix))
            {
                return panel;
            }
        }
    }

    return nullptr;
}