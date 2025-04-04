// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/ChoppingTable.h"
#include <Character/OC2Character.h>
#include <Global/OC2Enum.h>
#include "Global/GameMode/OC2GameMode.h"
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>

AChoppingTable::AChoppingTable()
{
	/*ComponentForProgressBar = CreateDefaultSubobject<USceneComponent>("ProgressBar");
	ComponentForProgressBar->SetupAttachment(RootComponent);*/

	ProgressBarComponent = CreateDefaultSubobject<UWidgetComponent>("ProgressBar");
	ProgressBarComponent->SetupAttachment(RootComponent);
	 

	/*static ConstructorHelpers::FClassFinder<UUserWidget> Widget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/LevelContent/Table/BP_ProgressBarWidget.BP_ProgressBarWidget''"));	
	if (true == Widget.Succeeded())
	{
		ProgressBarComponent->SetWidgetClass(Widget.Class);
		ProgressBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}*/

}

void AChoppingTable::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 클래스 지정
	ProgressBarComponent->SetWidgetClass(TSubClassWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = ProgressBarComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		WidgetPtr = Cast<UGaugeTextureWidget>(UserWidget);
	}
	ProgressBarComponent->SetDrawAtDesiredSize(true);   // 위젯의 실제 크기로 렌더
	ProgressBarComponent->SetPivot(FVector2D(0.5f, 0.5f)); // 중심 정렬
	ProgressBarComponent->SetWidgetSpace(EWidgetSpace::Screen); // 월드 공간에서 3D처럼 보이게
	ProgressBarComponent->bHiddenInGame = true;

	// 카메라를 향하도록 설정
	ProgressBarComponent->SetTwoSided(true);
	ProgressBarComponent->SetTickWhenOffscreen(true);
}

void AChoppingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == bTimerActivated)
	{
		TimerUpdate(DeltaTime);

		if (Timer > 2.0f)
		{
			bChoppingDone = true;
		}
	}
	Ratio += DeltaTime * 1.0f;

	WidgetPtr->SetProgressTimeRatio(Ratio);

	if (bChoppingDone == true)
	{
		ChoppingIsDone();
		FVector Loc = CookingPtr->GetActorLocation();
		int a = 0;
	}
}

ACooking* AChoppingTable::Interact(AActor* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (CookingPtr != nullptr && false == ChefPtr->IsHolding()) // 음식 있음, 셰프 빈 손
	{
		TempCooking = CookingPtr;
		CookingPtr = nullptr;
		return TempCooking;
	}
	else
	{
		return CookingPtr;
	}

}

void AChoppingTable::ChopIngredient(AActor* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);

	if (nullptr != ChefActor && nullptr != CookingPtr)
	{
		if (true == CookingPtr->IsCookingType(ECookingType::ECT_INGREDIENT))
		{
			AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);

			//if (true == PlacedIngredient->IsChoppable())
			{
				ChefPtr->Chopping(true);

				Timer = 0.0f;
				bTimerActivated = true;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "Chopping...");

				ProgressBarComponent->SetHiddenInGame(false);


				/*ProgressBar = GetWorld()->SpawnActor<ATableProgressBar>();
				ProgressBar->SetActorLocation(ComponentForProgressBar->GetComponentLocation());
				FVector Scale = CookingPtr->GetActorScale3D();
				ProgressBar->SetActorScale3D(Scale * 0.3);
				ProgressBar->AddActorWorldRotation({ 0.0, 00.0, -60.0 });*/
			}
		}
	}
}

void AChoppingTable::ChoppingIsDone()
{
	bTimerActivated = false;

	AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);
	PlacedIngredient->ChangeState(EIngredientState::EIS_CHOPPED);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, "Chopping Done");
	CookingPtr = Cast<ACooking>(PlacedIngredient);
	ProgressBarComponent->SetHiddenInGame(true);
	//ProgressBar->bDestroy = true;
	//ProgressBar = nullptr;

	ChefPtr->Chopping(false);
	ChefPtr = nullptr;
	bChoppingDone = false;
}

void AChoppingTable::TimerUpdate(float DeltaTime)
{
	Timer += DeltaTime;
	CurTime = Timer;
}

