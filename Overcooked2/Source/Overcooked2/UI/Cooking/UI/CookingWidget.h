#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "CookingWidget.generated.h"
UCLASS()
class OVERCOOKED2_API UCookingWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    // 주문 완료 처리
    void OrderComplete(int Index, int Score = 10);

    // 새로운 주문 생성
    void CreateNewOrder(struct FOrder& Order);

    void WrongOrder();
    //void SubmitWrongOrder();

    UFUNCTION(BlueprintCallable, Category = "OC2UI")
    void ShowReceiptWidget();

    void CheckFeverTime(int TipCount);

    void StartGame();
    void StartTimer();

    bool GetIsReady();

    bool IsTimesUp();
    bool IsShowScoreWidget();

    // UI 바인딩 (총 5개의 주문 슬롯)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_0 = nullptr;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_1 = nullptr;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_2 = nullptr;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_3 = nullptr;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* Order_4 = nullptr;


    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* ReadyCanvas = nullptr;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* GoCanvas = nullptr;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
    class UCanvasPanel* TimesUpCanvas = nullptr;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
    UCurveFloat* ColorCurve = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI", meta = (AllowPrivateAccess = "true"))
    bool bIsFinish = false;


    float StartTimerTick(float DeltaTime);

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
    TSubclassOf<UUserWidget> ScoreSubWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
    TSubclassOf<UUserWidget> TimeSubWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
    TSubclassOf<UUserWidget> FinalScoreSubWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
    TSubclassOf<UUserWidget> ReadySubWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
    TSubclassOf<UUserWidget> ReceiptWidget;

private:
    class UCookingScoreWidget* CookingScoreWidget = nullptr;
    class UCookingFinalScoreWidget* CookingFinalScoreWidget = nullptr;
    class UCookingReadyWidget* CookingReadyWidget = nullptr;
    class UCookingTimeWidget* CookingTimerWidget = nullptr;
    class UCookingReceiptWidget* CookingReceiptWidget = nullptr;

    // 주문 관련 변수
    int CompleteOrderNum = 0;
    int NewOrderNum = 0;
    int CurOrderCount = 0;

    float TimeLimit = 180.f;
    float ImageSize = 230.0f;
    float ImageOffset = 10.0f;
    float IngredientArrivePos = 100.f;
    float OpacityOffset = 0.02f;
    float FinalPos = 0.0f;
    float ArrivePos = 0.0f;

    float IngredientTimeElapsed = 0.0f;
    float MoveTimeElapsed = 0.0f;
    float ReadyTimeElapsed = 0.0f;
    float TimesUpTimeElapsed = 0.0f;
    float ReadyOffset = 0.0f;
    float TimesUpOffset = 0.0f;

    bool bShowScoreWidget = false;

    FVector2D TargetOffset = FVector2D(50.0f, 0.0f);
    FVector2D IngredientTargetOffset = FVector2D(0.0f, 10.0f);
    FVector2D IShortSize = { 72.0f, 72.0f };
    FVector2D ILongSize = { 72.0f, 115.f };

    TArray<class UCanvasPanel*> Orders;
    TArray<float> OrderTime;
    TArray<struct FOrder> CurOrders;

    // 타이머 핸들
    FTimerHandle OpacityTimerHandle;
    FTimerHandle MoveTimerHandle;
    FTimerHandle IngredientTimerHandle;
    FTimerHandle ReadyTimerHandle;
    FTimerHandle TimesUPTimerHandle;


    // 내부 함수
    void ShowReadyImageAnim();

    void UpdateImageOpacity();
    void UpdateImagePosition();
    void UpdateIngredientImagePosition();
    void UpdateOrderTime(int Index, float DeltaTime);
    void SettingIngredientImages(FOrder& order);
    void MoveNewOrder();
    void PlayReadyImageAnim();

    void FindOrderImgRecursive(class UWidget* Widget, const FLinearColor& Color);

    void ShowTimesUPAnim();
    void PlayTimesUPAnim();



    FTimeline WrongOrderTimeline;
    FTimeline CompleteOrderTimeline;

    FLinearColor OriginalColor = FLinearColor::White;
    FLinearColor TargetColor = FLinearColor::White;

    UFUNCTION()
    void UpdateAllOrderColor(float Value);

    UFUNCTION()
    void UpdateCompleteOrderColor(float Value);

    void UpdateImgColor(class UImage* Image, const FLinearColor& Color);
    void ResetColor();

    template <typename T>
    T* FindChildWidget(const FString& name, UCanvasPanel* canvas);

    class UImage* FindChildImage(const FString& name, UCanvasPanel* canvasPanel);
    class UCanvasPanel* FindChildPanel(const FString& name, UCanvasPanel* canvas);
};
