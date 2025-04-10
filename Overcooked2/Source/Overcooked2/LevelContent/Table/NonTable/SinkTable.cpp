// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/SinkTable.h"
#include "LevelContent/Cook/Plate.h"
#include <Character/OC2Character.h>
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>
#include <Net/UnrealNetwork.h>

ASinkTable::ASinkTable()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProgressBarComponent = CreateDefaultSubobject<UWidgetComponent>("ProgressBar");
	ProgressBarComponent->SetupAttachment(RootComponent);

	CleanPlateComponent = CreateDefaultSubobject<USceneComponent>("CleanPlate");
	CleanPlateComponent->SetupAttachment(RootComponent);

	{
		ComponentForDishes1 = CreateDefaultSubobject<USceneComponent>("DirtyPlate1");
		ComponentForDishes1->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes1);

		ComponentForDishes2 = CreateDefaultSubobject<USceneComponent>("DirtyPlate2");
		ComponentForDishes2->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes2);

		ComponentForDishes3 = CreateDefaultSubobject<USceneComponent>("DirtyPlate3");
		ComponentForDishes3->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes3);

		ComponentForDishes4 = CreateDefaultSubobject<USceneComponent>("DirtyPlate4");
		ComponentForDishes4->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes4);
	}
}

void ASinkTable::BeginPlay()
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

void ASinkTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckChefIsWashing();

	if (true == bTimerActivated)
	{
		Timer += DeltaTime;

		if (Timer > 2.0f)
		{
			bWashingDone = true;
		}
	}

	Ratio = (Timer / 0.4f) * 0.2f;
	WidgetPtr->SetProgressTimeRatio(Ratio);

	if (bWashingDone == true)
	{
		WashingIsDone();
	}
}

ACooking* ASinkTable::Interact(AActor* ChefActor)
{
	if (CleanPlates.Num() > 0)
	{
		CookingPtr = Cast<ACooking>(CleanPlates.Pop());
		return CookingPtr;
	}
	else
	{
		return nullptr;
	}
}

// 접시가 싱크대로 들어오는 로직
void ASinkTable::PlaceItem(ACooking* ReceivedCooking)
{
	if (ECookingType::ECT_PLATE == ReceivedCooking->GetCookingType())
	{
		APlate* TempPlate = Cast<APlate>(ReceivedCooking);
		if (nullptr == TempPlate)
		{
			return;
		}

		// 1. 더티 플레이트만 들어올 수 있다.
		if (true == TempPlate->IsDirtyPlate())
		{
			// 2-1. 플레이트가 하나만 들어온다면
			DirtyPlates.Add(TempPlate);

			// 2-2. 플레이트가 여러 개라면
			if (true != TempPlate->GetAnotherPlatesRef().IsEmpty())
			{
				int PlateNum = TempPlate->GetAnotherPlatesRef().Num();
		
				for (int i = 0; i < PlateNum; i++)
				{
					APlate* NewPlate = TempPlate->GetAnotherPlatesRef()[i];				
					DirtyPlates.Add(NewPlate);
				}
			}

			// 3. 정렬한다.
			for (int32 i = CurPlateNum; i < DirtyPlates.Num(); i++)
			{
				APlate* NewPlate = DirtyPlates[i];
				NewPlate->SetCookingTable_Implementation(this);
				NewPlate->AttachToComponent(DirtyPlateComponents[i], FAttachmentTransformRules::KeepRelativeTransform);
				NewPlate->SetActorLocation(DirtyPlateComponents[i]->GetComponentLocation());
				//DirtyPlates[i + CurPlateNum]->SetCookingTable_Implementation(this);
				//DirtyPlates[i + CurPlateNum]->AttachToComponent(DirtyPlateComponents[i + CurPlateNum], FAttachmentTransformRules::KeepRelativeTransform);
				//DirtyPlates[i + CurPlateNum]->SetActorLocation(DirtyPlateComponents[i + CurPlateNum]->GetComponentLocation());
			}

			CurPlateNum = DirtyPlates.Num();

			// i + curPlateNum이 4를 안 넘을지?

			//CookingPtr = ReceivedCooking;
			//CookingPtr->SetCookingTable_Implementation(this);
			//CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
			//CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());
		}
	}
}

void ASinkTable::DoTheDishes(AOC2Character* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);

	if (nullptr != ChefActor && false == DirtyPlates.IsEmpty())
	{
		ChefPtr->Washing(true);

		Timer = 0.0f;
		bTimerActivated = true;
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "Washing...");

		HideProgressBar(false);
		//ProgressBarComponent->SetHiddenInGame(false);
	}
}

void ASinkTable::WashingIsDone()
{
	bTimerActivated = false;

	DirtyPlates.Last()->WashPlate();
	CleanPlates.Add(DirtyPlates.Pop());
	CurPlateNum = DirtyPlates.Num();

	int PlateNum = CleanPlates.Num();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, "Washing Done");

	if (PlateNum - 1 < 0)
	{
		return;
	}

	CleanPlates[PlateNum -1]->AttachToComponent(CleanPlateComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CleanPlates[PlateNum -1]->SetActorLocation(CleanPlateComponent->GetComponentLocation());
	CleanPlates[PlateNum - 1]->AddActorLocalOffset(FVector::UnitZ() * 30.0f * (PlateNum -1));

	//APlate* PlacedPlate = Cast<APlate>(CookingPtr);
	//PlacedPlate->WashPlate();
	//CookingPtr = Cast<APlate>(PlacedPlate);

	//CookingPtr->DetachAllSceneComponents();
	//ProgressBarComponent->SetHiddenInGame(true);
	HideProgressBar(true);

	if (DirtyPlates.Num() == 0 || false == ChefPtr->IsWashing())
	{
		ChefPtr->Washing(false);
		ChefPtr = nullptr;
		bWashingDone = false;
	}
	else
	{
		DoTheDishes(ChefPtr);
	}
	
}

void ASinkTable::HideProgressBar_Implementation(bool Value)
{
	ProgressBarComponent->SetHiddenInGame(Value);
}

void ASinkTable::CheckChefIsWashing()
{
	if (nullptr != ChefPtr)
	{
		if (false == ChefPtr->IsWashing())
		{
			bTimerActivated = false;
			ChefPtr = nullptr;
			//ProgressBarComponent->SetHiddenInGame(true);
			HideProgressBar(true);
		}
	}
}

void ASinkTable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASinkTable, ChefPtr);
	DOREPLIFETIME(ASinkTable, Timer);
	DOREPLIFETIME(ASinkTable, bTimerActivated);
	DOREPLIFETIME(ASinkTable, bWashingDone);
	DOREPLIFETIME(ASinkTable, Ratio);
	DOREPLIFETIME(ASinkTable, ComponentForDishes1);
	DOREPLIFETIME(ASinkTable, ComponentForDishes2);
	DOREPLIFETIME(ASinkTable, ComponentForDishes3);
	DOREPLIFETIME(ASinkTable, ComponentForDishes4);
	DOREPLIFETIME(ASinkTable, DirtyPlateComponents);
	DOREPLIFETIME(ASinkTable, DirtyPlates);
	DOREPLIFETIME(ASinkTable, CleanPlates);
}