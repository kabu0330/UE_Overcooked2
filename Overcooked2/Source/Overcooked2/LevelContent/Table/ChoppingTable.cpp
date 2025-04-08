// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/ChoppingTable.h"
#include <Character/OC2Character.h>
#include <Global/OC2Enum.h>
#include "Global/GameMode/OC2GameMode.h"
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>
#include <Net/UnrealNetwork.h>

AChoppingTable::AChoppingTable()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProgressBarComponent = CreateDefaultSubobject<UWidgetComponent>("ProgressBar");
	ProgressBarComponent->SetupAttachment(RootComponent);

	KnifeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Knife");
	KnifeMeshComponent->SetupAttachment(RootComponent);
	KnifeMeshComponent->SetIsReplicated(true);
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
	CheckChefIsChopping();

	if (true == bTimerActivated)
	{
		TimerUpdate(DeltaTime);

		if (Timer > 2.0f)
		{
			bChoppingDone = true;
		}
	}

	Ratio = (Timer / 0.4f) * 0.2f;
	WidgetPtr->SetProgressTimeRatio(Ratio);

	if (bChoppingDone == true)
	{
		ChoppingIsDone();
	}

	HideKnife();
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

void AChoppingTable::HideKnife_Implementation()
{
	if (nullptr != CookingPtr)
	{
		bCheckHidden = true;
	}
	else
	{
		bCheckHidden = false;
	}

	if (true == bCheckHidden)
	{
		KnifeMeshComponent->SetHiddenInGame(true);
	}
	else
	{
		KnifeMeshComponent->SetHiddenInGame(false);
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

			if (true == PlacedIngredient->IsChoppable())
			{
				ChefPtr->Chopping(true);

				Timer = 0.0f;
				bTimerActivated = true;
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "Chopping...");

				ShowProgressBar(false);
			}
		}
	}
}

void AChoppingTable::ShowProgressBar_Implementation(bool Value)
{
	ProgressBarComponent->SetHiddenInGame(Value);
}

void AChoppingTable::ChoppingIsDone()
{
	bTimerActivated = false;

	AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);
	PlacedIngredient->ChangeState(EIngredientState::EIS_CHOPPED);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, "Chopping Done");
	CookingPtr = Cast<ACooking>(PlacedIngredient);
	ProgressBarComponent->SetHiddenInGame(true);

	ChefPtr->Chopping(false);
	ChefPtr = nullptr;
	bChoppingDone = false;
}

void AChoppingTable::TimerUpdate(float DeltaTime)
{
	Timer += DeltaTime;
	CurTime = Timer;
}

void AChoppingTable::CheckChefIsChopping()
{
	if (nullptr != ChefPtr)
	{
		if (false == ChefPtr->IsCooking())
		{
			bTimerActivated = false;
			ChefPtr = nullptr;
			ProgressBarComponent->SetHiddenInGame(true);
		}
	}
}

void AChoppingTable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChoppingTable, bCheckHidden);
	DOREPLIFETIME(AChoppingTable, ProgressBarComponent);
	DOREPLIFETIME(AChoppingTable, Ratio);
	DOREPLIFETIME(AChoppingTable, Timer);
	DOREPLIFETIME(AChoppingTable, bTimerActivated);
	DOREPLIFETIME(AChoppingTable, bChoppingDone);
}
