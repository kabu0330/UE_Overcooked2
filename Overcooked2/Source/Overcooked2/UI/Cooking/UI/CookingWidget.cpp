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


void UCookingWidget::OrderComplete(int Index)
{
    if (Index >= Orders.Num()) return;

    CompleteOrderNum = Index;

    if (Orders[CompleteOrderNum] == nullptr) return;

    if (GetWorld()->GetTimerManager().IsTimerActive(OpacityTimerHandle)) return;

    if (Orders[CompleteOrderNum]->GetVisibility() != ESlateVisibility::Collapsed)
    {
        CurOrderCount -= 1;

        UCanvasPanel* Panel = FindChildPanel("IngredientPanel_", Orders[CompleteOrderNum]);
        UCanvasPanel* ImgPanel = FindChildPanel("I_Img_", Panel);
        UCanvasPanel* IBackPanel = FindChildPanel("IBackImg_", Panel);

        if (ImgPanel == nullptr || Panel == nullptr || IBackPanel == nullptr) return;

        UImage* ToolImg = FindChildImage("Tool_", ImgPanel);
        ToolImg->SetVisibility(ESlateVisibility::Hidden);

        UImage* BImg = FindChildImage("BIngredient1", IBackPanel);
        UCanvasPanelSlot* BackSlot = Cast<UCanvasPanelSlot>(BImg->Slot);
        BackSlot->SetSize(IShortSize);

        Panel->SetRenderTranslation({ 0.0f, -IngredientArrivePos });

        GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
    }

}

void UCookingWidget::CreateNewOrder(FOrder& Order)
{

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

    UProgressBar* TimeImg = FindChildWidget<UProgressBar>("Time_", Orders[NewOrderNum]);
    TimeImg->SetFillColorAndOpacity({ 0.1f,0.3f,0.0f });

    CurOrderCount += 1;

    {

        UImage* DishImage = FindChildImage("Dish_", Orders[NewOrderNum]);
        if (DishImage == nullptr) return;

        if (Order.OrderTexutre != nullptr)
        {
            DishImage->SetBrushFromTexture(Order.OrderTexutre);
        }
        else
        {
            FString TexturePath = TEXT("/Game/Resources/UI/Order/Dish/ui_cheeseburger.ui_cheeseburger");
            class UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

            DishImage->SetBrushFromTexture(Texture);
        }

    }

    SettingIngredientImages(Order);

    GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
}

void UCookingWidget::FindOrderImgRecursive(UWidget* Widget, const FLinearColor& Color)
{
    if (!Widget) return;

    if (UImage* ImageWidget = Cast<UImage>(Widget))
    {
        ImageWidget->SetBrushTintColor(FSlateColor(Color));
        //UpdateImgColor(ImageWidget, Color);

        return;
    }

    if (UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(Widget))
    {
        TArray<UWidget*> Children = CanvasPanel->GetAllChildren();
        for (UWidget* Child : Children)
        {
            FindOrderImgRecursive(Child, Color);
        }
    }
}

void UCookingWidget::UpdateImgColor(UImage* Image, const FLinearColor& Color)
{
    if (nullptr == Image) return;


}


void UCookingWidget::WrongOrder()
{
    for (UCanvasPanel* Order : Orders)
    {
        if (!Order) continue;

        FindOrderImgRecursive(Order, FLinearColor::Red);
    }

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            for (UCanvasPanel* Order : Orders)
            {
                if (!Order) continue;

                FindOrderImgRecursive(Order, FLinearColor::White);
            }
        }, 0.5f, false);
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
    else if (NewOrderNum == 0)
    {
        ArrivePos = ImageOffset;
    }

    if (CurPos.X <= ArrivePos && CurAngle >= 0.0f)
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
        if (CurPos.X > ArrivePos)
        {
            Orders[NewOrderNum]->SetRenderTranslation(CurPos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));

        }
        if (CurAngle < 0.0f)
        {
            Orders[NewOrderNum]->SetRenderTransformAngle(CurAngle + 0.7f);
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

            if (Order.RequireIngredients[i].IngredientState == EIngredientState::EIS_BOILABLE)
            {
                UImage* Toolimg = FindChildImage("Tool_", ImgPanel);
                Toolimg->SetVisibility(ESlateVisibility::Visible);
                BackSlot->SetSize(ILongSize);
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
        for (int i = Order.RequireIngredients.Num() - 1; i < 3; i++)
        {
            UImage* Img = FindChildImage("IngredientImg" + FString::FromInt(i), ImgPanel);
            UImage* BImg = FindChildImage("BIngredient" + FString::FromInt(i), IBackPanel);
            Img->SetVisibility(ESlateVisibility::Hidden);
            BImg->SetVisibility(ESlateVisibility::Hidden);
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


    if (OrderTime[Index] > (TimeLimit / 3) * 2)
    {
        TimeImg->SetFillColorAndOpacity({ Color.R + DeltaTime * 0.7f, Color.G + DeltaTime * 0.7f, 0.0f });
    }
    else if (OrderTime[Index] > TimeLimit / 3)
    {
        TimeImg->SetFillColorAndOpacity({ Color.R + DeltaTime * 0.7f, Color.G - DeltaTime * 0.5f, 0.0f });
    }
    else
    {
        TimeImg->SetFillColorAndOpacity({ Color.R + DeltaTime * 0.2f, Color.G - DeltaTime * 0.1f, 0.0f });
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

    if (Orders[CompleteOrderNum]->GetRenderOpacity() <= 0.0f)
    {
        Orders[CompleteOrderNum]->SetVisibility(ESlateVisibility::Collapsed);
        UpdateImagePosition();

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