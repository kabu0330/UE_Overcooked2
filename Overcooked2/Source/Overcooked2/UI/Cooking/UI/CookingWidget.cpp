// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Global/Data/OrderDataTable.h"


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
}

void UCookingWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    for (int i = 0; i < CurOrderCount; i++)
    {
        UpdateOrderTime(i, DeltaTime);
    }
}


void UCookingWidget::OrderComplete(int index)
{

    CompleteOrderNum = index;

    if (Orders[CompleteOrderNum] == nullptr) return;

    if (GetWorld()->GetTimerManager().IsTimerActive(OpacityTimerHandle)) return;

    if (Orders[CompleteOrderNum]->GetVisibility() != ESlateVisibility::Collapsed)
    {
        CurOrderCount -= 1;

        UCanvasPanel* panel = FindChildPanel("IngredientPanel_", Orders[CompleteOrderNum]);
        UCanvasPanel* imgpanel = FindChildPanel("I_Img_", panel);
        UCanvasPanel* ibackpanel = FindChildPanel("IBackImg_", panel);

        if (imgpanel == nullptr || panel == nullptr || ibackpanel == nullptr) return;

        UImage* toolimg = FindChildImage("Tool_", imgpanel);
        toolimg->SetVisibility(ESlateVisibility::Hidden);

        UImage* bimg = FindChildImage("BIngredient1", ibackpanel);
        UCanvasPanelSlot* backslot = Cast<UCanvasPanelSlot>(bimg->Slot);
        backslot->SetSize(IShortSize);

        panel->SetRenderTranslation({ 0.0f, -IngredientArrivePos });

        GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
    }

}

void UCookingWidget::CreateNewOrder(FOrder& order)
{
    int Test1 = CurOrderCount;
    int Test2 = Orders.Num();

    if (CurOrderCount >= Orders.Num()) return;

    if (GetWorld()->GetTimerManager().IsTimerActive(MoveTimerHandle)) return;

    NewOrderNum = CurOrderCount;

    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);

    Orders[NewOrderNum]->SetVisibility(ESlateVisibility::Visible);
    Orders[NewOrderNum]->SetRenderTransformAngle(-20.0f);
    Orders[NewOrderNum]->SetRenderOpacity(1.0f);
    Orders[NewOrderNum]->SetRenderTranslation({ ScreenSize.X * 1.5f, 0.0f });
    Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.2f });
    OrderTime[NewOrderNum] = TimeLimit;

    UProgressBar* timeimg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);
    timeimg->SetFillColorAndOpacity({ 0.1f,0.3f,0.0f });

    CurOrderCount += 1;

    {

        UImage* dishimage = FindChildImage("Dish_", Orders[NewOrderNum]);
        if (dishimage == nullptr) return;

        if (order.OrderTexutre != nullptr)
        {
            dishimage->SetBrushFromTexture(order.OrderTexutre);
        }
        else
        {
            FString texturepath = TEXT("/Game/Resources/UI/Order/Dish/ui_cheeseburger.ui_cheeseburger");
            class UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

            dishimage->SetBrushFromTexture(texture);
        }

    }

    SettingIngredientImages(order);

    GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
}



void UCookingWidget::MoveNewOrder()
{
    if (Orders[NewOrderNum] == nullptr) return;

    FVector2D curpos = Orders[NewOrderNum]->GetRenderTransform().Translation;

    float curangle = Orders[NewOrderNum]->GetRenderTransform().Angle;

    if (NewOrderNum != 0)
    {
        ArrivePos = ImageOffset;
        for (int i = 0; i < CurOrderCount - 1; i++)
        {
            UImage* img = FindChildImage("OrderBackground_", Orders[i]);

            float scale = img->GetRenderTransform().Scale.X;
            ArrivePos += ImageSize * scale + ImageOffset;
        }

    }
    else if (NewOrderNum == 0)
    {
        ArrivePos = ImageOffset;
    }

    if (curpos.X <= ArrivePos && curangle >= 0.0f)
    {
        Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.0f });
        Orders[NewOrderNum]->SetRenderTranslation({ ArrivePos,0.0f });
        Orders[NewOrderNum]->SetRenderTransformAngle({ 0.0f });

        MoveTimeElapsed = 0.0f;
        IngredientTimeElapsed = 0.0f;

        if (GetWorld()->GetTimerManager().IsTimerActive(IngredientTimerHandle)) return;

        GetWorld()->GetTimerManager().ClearTimer(IngredientTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(IngredientTimerHandle, this, &UCookingWidget::UpdateIngredientImagePosition, 0.01f, true);

        return;
    }
    else
    {
        if (curpos.X > ArrivePos)
        {
            Orders[NewOrderNum]->SetRenderTranslation(curpos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));

        }
        if (curangle < 0.0f)
        {
            Orders[NewOrderNum]->SetRenderTransformAngle(curangle + 0.7f);
        }
    }

    MoveTimeElapsed += 0.1f;
}

void UCookingWidget::SettingIngredientImages(FOrder& order)
{
    if (order.RequireIngredients.IsEmpty())
    {
        return;
    }


    UCanvasPanel* panel = FindChildPanel("IngredientPanel_", Orders[NewOrderNum]);
    if (panel == nullptr) return;

    UCanvasPanel* ibackpanel = FindChildPanel("IBackImg_", panel);
    if (ibackpanel == nullptr) return;

    UCanvasPanel* imgpanel = FindChildPanel("I_Img_", panel);
    if (imgpanel == nullptr) return;

    int testnum = 3;

    if (NewOrderNum == 1)
    {
        testnum = 2;
    }


    //for (int i = 0; i < testnum; i++)
    for (int i = 0; i < order.RequireIngredients.Num(); i++)
    {
        TArray<UWidget*> backpanelchind = ibackpanel->GetAllChildren();
        UWidget* ibackimg = backpanelchind[i];


        if (ibackimg != nullptr)
        {
            UCanvasPanelSlot* backslot = Cast<UCanvasPanelSlot>(ibackimg->Slot);
            UImage* bimg = FindChildImage("BIngredient" + FString::FromInt(i), ibackpanel);
            UImage* img = FindChildImage("IngredientImg" + FString::FromInt(i), imgpanel);
            img->SetVisibility(ESlateVisibility::Visible);
            bimg->SetVisibility(ESlateVisibility::Visible);

            UImage* dishimg = FindChildImage("Dish_", Orders[NewOrderNum]);
            dishimg->SetRenderTranslation({ 0.0f, 0.0f });

            UImage* backimg = FindChildImage("OrderBackground_", Orders[NewOrderNum]);
            backimg->SetRenderScale({ 1.0f, 1.0f });
            backimg->SetRenderTranslation({ 0.f, 0.0f });

            UProgressBar* timeimg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);
            timeimg->SetRenderScale({ 1.0f, 1.0f });
            timeimg->SetRenderTranslation({ 0.f, 0.0f });

            //if (i == 1)
            if (order.RequireIngredients[i].IngredientState == EIngredientState::EIS_BOILABLE)
            {
                UImage* toolimg = FindChildImage("Tool_", imgpanel);
                toolimg->SetVisibility(ESlateVisibility::Visible);
                backslot->SetSize(ILongSize);
            }

            
            if (order.RequireIngredients[i].IngredientTexture != nullptr)
            {
                img->SetBrushFromTexture(order.RequireIngredients[i].IngredientTexture);
            }
            else
            {
                FString texturepath = TEXT("/Game/Resources/UI/Order/Ingredient/Fish_Icon.Fish_Icon");
                class UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

                img->SetBrushFromTexture(texture);
            }
        }
    }

    //if (testnum < 3)
    if (order.RequireIngredients.Num() < 3)
    {
        //for (int i = testnum; i < 3; i++)
        for (int i = order.RequireIngredients.Num() - 1; i < 3; i++)
        {
            UImage* img = FindChildImage("IngredientImg" + FString::FromInt(i), imgpanel);
            UImage* bimg = FindChildImage("BIngredient" + FString::FromInt(i), ibackpanel);
            img->SetVisibility(ESlateVisibility::Hidden);
            bimg->SetVisibility(ESlateVisibility::Hidden);
        }


        UImage* dishimg = FindChildImage("Dish_", Orders[NewOrderNum]);
        UImage* backimg = FindChildImage("OrderBackground_", Orders[NewOrderNum]);
        UProgressBar* timeimg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);

        dishimg->SetRenderTranslation({ -30.0f, 0.0f });

        backimg->SetRenderScale({ 0.7f, 1.0f });
        backimg->SetRenderTranslation({ -30.f, 0.0f });

        timeimg->SetRenderScale({ 0.7f, 1.0f });
        timeimg->SetRenderTranslation({ -30.f, 0.0f });
    }

}


void UCookingWidget::UpdateOrderTime(int Index, float DeltaTime)
{
    OrderTime[Index] -= DeltaTime;

    UProgressBar* timeimg = FindChildWidget<UProgressBar>("Time_", Orders[Index]);

    timeimg->SetPercent({ OrderTime[Index] / TimeLimit });
    FLinearColor color = timeimg->GetFillColorAndOpacity();


    if (OrderTime[Index] > (TimeLimit / 3) * 2)
    {
        timeimg->SetFillColorAndOpacity({ color.R + DeltaTime * 0.7f, color.G + DeltaTime * 0.7f, 0.0f });
    }
    else if (OrderTime[Index] > TimeLimit / 3)
    {
        timeimg->SetFillColorAndOpacity({ color.R + DeltaTime * 0.7f, color.G - DeltaTime * 0.5f, 0.0f });
    }
    else
    {
        timeimg->SetFillColorAndOpacity({ color.R + DeltaTime * 0.2f, color.G - DeltaTime * 0.1f, 0.0f });
    }

}


void UCookingWidget::UpdateIngredientImagePosition()
{
    if (Orders[NewOrderNum] == nullptr) return;

    UCanvasPanel* panel = FindChildPanel("IngredientPanel_", Orders[NewOrderNum]);

    if (panel == nullptr) return;

    FVector2D curpos = panel->GetRenderTransform().Translation;

    if (curpos.Y >= IngredientArrivePos)
    {
        panel->SetRenderTranslation({ 0.0f, IngredientArrivePos });

        IngredientTimeElapsed = 0.0f;

        GetWorld()->GetTimerManager().ClearTimer(IngredientTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;

    }
    else
    {
        panel->SetRenderTranslation(curpos + FVector2D(0.0f, IngredientTargetOffset.Y + IngredientTimeElapsed));
    }


    IngredientTimeElapsed += 0.1f;
}


void UCookingWidget::UpdateImageOpacity()
{

    if (Orders[CompleteOrderNum]->GetRenderOpacity() <= 0.0f)
    {
        Orders[CompleteOrderNum]->SetVisibility(ESlateVisibility::Collapsed);
        UpdateImagePosition();

        GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
        return;
    }

    float curopacity = Orders[CompleteOrderNum]->GetRenderOpacity();
    Orders[CompleteOrderNum]->SetRenderOpacity(FMath::Clamp(curopacity - OpacityOffset, 0.0f, 1.0f));


    TArray<UWidget*> Children = Orders[CompleteOrderNum]->GetAllChildren();

    for (UWidget* Child : Children)
    {
        // UImage 타입인지 확인하고 opacity 적용
        if (UImage* Image = Cast<UImage>(Child))
        {
            const FLinearColor& color = Image->GetColorAndOpacity();
            Image->SetColorAndOpacity({ color.R, color.G + 1.0f, color.B, color.A });
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
            UImage* img = FindChildImage("OrderBackground_", Orders[j]);
            float scale = img->GetRenderTransform().Scale.X;

            FinalPos += ImageSize * scale + ImageOffset;
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
        if (UImage* image = Cast<UImage>(Child))
        {
            FString imgname = image->GetName();
            if (imgname.StartsWith(TargetPrefix))
            {
                return image;
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