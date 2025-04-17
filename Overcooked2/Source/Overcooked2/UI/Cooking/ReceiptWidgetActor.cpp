// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/ReceiptWidgetActor.h"
#include "Components/WidgetComponent.h"
#include "UI/Cooking/UI/CookingReceiptWidget.h"

// Sets default values
AReceiptWidgetActor::AReceiptWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World); 
	WidgetComponent->SetDrawSize(FVector2D(3000.f, 1500.f));

	RootComponent = WidgetComponent;

}

// Called when the game starts or when spawned
void AReceiptWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReceiptWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReceiptWidgetActor::ShowTotalScoreAnim()
{
	if (nullptr != WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
		if (Widget)
		{
			CookingReceiptWidget = Cast<UCookingReceiptWidget>(Widget);
			if (CookingReceiptWidget)
			{
				CookingReceiptWidget->ShowScoreText();
				CookingReceiptWidget->CheckStar();
			}
		}
	}
}