// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/SinkTable.h"
#include "LevelContent/Cook/Plate.h"
#include <Character/OC2Character.h>
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>
#include <Global/Component/TimeEventComponent.h>
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
		ComponentForDishes1 = CreateDefaultSubobject<UStaticMeshComponent>("DirtyPlate1");
		ComponentForDishes1->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes1);

		ComponentForDishes2 = CreateDefaultSubobject<UStaticMeshComponent>("DirtyPlate2");
		ComponentForDishes2->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes2);

		ComponentForDishes3 = CreateDefaultSubobject<UStaticMeshComponent>("DirtyPlate3");
		ComponentForDishes3->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes3);

		ComponentForDishes4 = CreateDefaultSubobject<UStaticMeshComponent>("DirtyPlate4");
		ComponentForDishes4->SetupAttachment(ComponentForCooking);
		DirtyPlateComponents.Add(ComponentForDishes4);
	}

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent"));

}

void ASinkTable::BeginPlay()
{
	Super::BeginPlay();

	InitProgressBar();

	SetAllPlateHidden();
}


void ASinkTable::InitProgressBar()
{
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

ACooking* ASinkTable::Interact(AActor* ChefActor)
{
	// 설거지된 접시가 하나 이상 있으면 캐릭터에게 하나를 줄 수 있다.
	if (CleanPlates.Num() > 0)
	{
		APlate* NewPlate = CleanPlates.Last();
		CookingPtr = Cast<ACooking>(NewPlate);
		if (nullptr != CookingPtr)
		{
			CleanPlates.Pop();

			CleanPlateNum = CleanPlates.Num();
			return CookingPtr;
		}
		else
		{
			return nullptr;
		}

	}
	else
	{
		return nullptr;
	}
}

// 접시가 싱크대로 들어오는 로직
void ASinkTable::PlaceItem(ACooking* ReceivedCooking)
{
	PlacePlates(ReceivedCooking);
}

// 접시가 싱크대로 들어오는 로직
void ASinkTable::PlacePlates_Implementation(ACooking* ReceivedCooking)
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
					NewPlate->ResetForCleaning();

					DirtyPlates.Add(NewPlate);
				}
			}

			// AnotherPlateRef도 정리, 메시 초기화, 보유한 Plates 초기화
			TempPlate->ResetForCleaning();

			// 3. 렌더링한다.
			SetPlateVisibility(DirtyPlates.Num());

			// 4. Plates 중 월드에서 제외된 녀석들을 월드로 복귀시킨다.
			for (int32 i = DirtyPlateNum; i < DirtyPlates.Num(); i++)
			{
				APlate* NewPlate = DirtyPlates[i];
				NewPlate->HiddenPlateToWorld();
				NewPlate->SetCookingTable_Implementation(this);
			}

			DirtyPlateNum = DirtyPlates.Num();
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
		//KeepWashing = true;
		//ProgressBarComponent->SetHiddenInGame(false);
	}
}

void ASinkTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 셰프가 나와의 상호작용을 중단하고 떠났는지
	CheckChefIsWashing();

	UpdateProgressBar(DeltaTime);

	WashingIsDone();

	// 1. 설거지가 끝났을 때
	if (true == bWashingDone && false == DirtyPlates.IsEmpty())
	{
		DoTheDishes(ChefPtr); // 셰프가 처음에 한 번 불러준다. 이후에는 내가 불러줌
		bWashingDone = false;
		return;
	}
	else if (true == bWashingDone)
	{
		// 설거지 모션 중단
		ChefPtr->Washing(false);
		bWashingDone = false;
		return;
	}
}

void ASinkTable::CheckChefIsWashing()
{
	// 셰프가 나와 상호작용을 하던 중에
	if (nullptr != ChefPtr)
	{
		// 다른 행동으로 상호작용이 중단되었다면
		if (false == ChefPtr->IsWashing())
		{
			bTimerActivated = false;
			ChefPtr = nullptr;
			HideProgressBar(true);
			//ProgressBarComponent->SetHiddenInGame(true);
		}
	}
}

void ASinkTable::UpdateProgressBar(float DeltaTime)
{
	if (true == bTimerActivated /*&& true == KeepWashing*/)
	{
		Timer += DeltaTime;

		if (Timer > 2.0f)
		{
			// 트리거 변수
			bWashingDone = true;
		}
	}

	Ratio = (Timer / 0.4f) * 0.2f;
	WidgetPtr->SetProgressTimeRatio(Ratio);
}

// 설거지 완료했을 때 한 번 호출, 싱크대로 접시를 하나 이동시킨다.
void ASinkTable::WashingIsDone_Implementation()
{
	// 설거지 중이라면 리턴
	if (false == bWashingDone)
	{
		return;
	}

	bTimerActivated = false; // 타이머 끄고

	if (0 == DirtyPlates.Num())
	{
		return;
	}

	if (nullptr != GetWorld()->GetAuthGameMode())
	{
		int a = 0;
	}
	if (nullptr == GetWorld()->GetAuthGameMode())
	{
		int a = 0;
	}

	// 인덱스의 마지막 접시를 empty 상태로 바꾸고
	APlate* NewPlate = DirtyPlates.Last();
	
	NewPlate->RestorePlateToWorld(); // 월드 재편입
	NewPlate->WashPlate(); // 초기 상태로 만든다.

	// CleanPlates에 추가, DirtyPlates의 마지막 접시 삭제
	CleanPlates.Add(NewPlate);
	DirtyPlates.Pop();

	// 싱크대의 플레이트 렌더링 상태를 바꿔줘야한다.
	//for (size_t i = 0; i < DirtyPlates.Num(); i++)
	//{

	//}

	DirtyPlateNum = DirtyPlates.Num();
	SetPlateVisibility(DirtyPlateNum);

	CleanPlateNum = CleanPlates.Num();

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, "Washing Done");

	if (CleanPlateNum == 0)
	{
		return;
	}

	// 클린 플레이트를 싱크대에 부착시킨다.
	NewPlate->AttachToComponent(CleanPlateComponent, FAttachmentTransformRules::KeepRelativeTransform);
	NewPlate->SetActorLocation(CleanPlateComponent->GetComponentLocation());

	// 클린 플레이트의 Num이 증가하면 쌓아준다.
	NewPlate->AddActorLocalOffset(FVector::UnitZ() * 10.0f * (CleanPlateNum - 1));


	//APlate* PlacedPlate = Cast<APlate>(CookingPtr);
	//PlacedPlate->WashPlate();
	//CookingPtr = Cast<APlate>(PlacedPlate);

	//CookingPtr->DetachAllSceneComponents();
	//ProgressBarComponent->SetHiddenInGame(true);
	HideProgressBar(true);
}

void ASinkTable::HideProgressBar_Implementation(bool Value)
{
	ProgressBarComponent->SetHiddenInGame(Value);
}

void ASinkTable::SetPlateVisibility/*_Implementation*/(int Index)
{
	SetAllPlateHidden();
	for (size_t i = 0; i < Index; i++)
	{
		DirtyPlateComponents[i]->SetVisibility(true);
	}
}

void ASinkTable::SetAllPlateHidden/*_Implementation*/()
{
	for (int32 i = 0; i < DirtyPlateComponents.Num(); i++)
	{
		DirtyPlateComponents[i]->SetVisibility(false);
	}
}

void ASinkTable::SetPlateVisibilityWithIndex/*_Implementation*/(int Index, bool Value)
{
	DirtyPlateComponents[Index]->SetVisibility(Value);
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
	//DOREPLIFETIME(ASinkTable, DirtyPlates);
	//DOREPLIFETIME(ASinkTable, CleanPlates);
}