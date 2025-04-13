// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/SinkTable.h"
#include "LevelContent/Cook/Plate.h"
#include <Character/OC2Character.h>
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>
#include <Global/Component/TimeEventComponent.h>
#include <Net/UnrealNetwork.h>
#include <Global/OC2Global.h>
#include <Global/Data/OC2GlobalData.h>

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

	CleanPlateMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CleanPlateNeshComponent"));
	CleanPlateMeshComponent->SetupAttachment(RootComponent);

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent"));

}

void ASinkTable::BeginPlay()
{
	Super::BeginPlay();

	InitProgressBar();

	InitDirtyPlateMesh();
	InitCleanPlateMesh();

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

void ASinkTable::InitDirtyPlateMesh()
{
	for (int32 i = 0; i < DirtyPlateComponents.Num(); i++)
	{
		DirtyPlateComponents[i]->SetIsReplicated(true);
	}

	SetAllPlateHidden();
}

void ASinkTable::InitCleanPlateMesh()
{
	CleanPlateMeshComponent->AttachToComponent(CleanPlateComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CleanPlateMeshComponent->SetRelativeLocation(FVector(0, 10, 60));
	CleanPlateMeshComponent->SetRelativeScale3D(FVector(2, 2, 2));
	SetCleanPlateMesh();
}

ACooking* ASinkTable::Interact(AActor* ChefActor)
{
	// 설거지된 접시가 하나 이상 있으면 캐릭터에게 하나를 줄 수 있다.
	if (0 >= CleanPlateNum)
	{
		return nullptr;
	}

	if (true == HasAuthority())
	{
		AddCleanPlateNum(-1);
		APlate* NewPlate = UOC2Global::GetPlate(GetWorld());
		if (nullptr != NewPlate)
		{
			NewPlate->RestorePlateToWorld();
			NewPlate->CleanPlate();
			SetCleanPlateMesh();
			return NewPlate;
		}
	}



	return nullptr;
}

// 접시가 싱크대로 들어오는 로직
void ASinkTable::PlaceItem(ACooking* ReceivedCooking)
{
	PlacePlates(ReceivedCooking);
}

// 접시가 싱크대로 들어오는 로직
void ASinkTable::PlacePlates_Implementation(ACooking* ReceivedCooking)
{
	if (nullptr == ReceivedCooking)
	{
		return;
	}

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
			// 싱크대는 접시의 포인터를 가지지 않는다. 접시가 들어오고 캐릭터가 가져간 개수만 기억한다.
			//										N - 1개 + 1개 = N개
			AddDirtyPlateNum(TempPlate->GetPlateStackCount() + 1);
			TempPlate->HiddenPlateToWorld();
			UOC2Global::MovePlate(GetWorld(), TempPlate); // Root Plate도 GameState로 보낸다.
			// 싱크대는 접시의 포인터가 없다. 메시 컴포넌트를 통해서 눈속임만 한다.

			if (true == HasAuthority())
			{
				SetPlateVisibility(DirtyPlateNum); // Render
			}
		}
	}
}

void ASinkTable::DoTheDishes(AOC2Character* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);

	// 셰프가 싱크대를 떠나지 않았고 설거지할 접시가 남아있다면
	if (nullptr != ChefActor && 0 < DirtyPlateNum)
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

	// 1. 설거지가 끝났을 때 아직 설거지 할 접시가 남아있다면 다시 설거지 로직 호출
	if (true == bWashingDone && 0 < DirtyPlateNum)
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

	if (0 >= DirtyPlateNum)
	{
		DirtyPlateNum = 0;
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, "Washing Done");

	HideProgressBar(true);
	
	if (true == HasAuthority())
	{
		// Dirty -1 , Clean + 1
		AddDirtyPlateNum(-1);
		AddCleanPlateNum(1);
	}

	// 싱크대 안에 있는 Dirty Plate 개수 렌더링
	SetPlateVisibility(DirtyPlateNum); 

	// 싱크대 위에 있는 Clean Plate 개수 렌더링
	SetCleanPlateMesh();
}

void ASinkTable::HideProgressBar_Implementation(bool Value)
{
	ProgressBarComponent->SetHiddenInGame(Value);
}

void ASinkTable::SetPlateVisibility_Implementation(int Index)
{
	SetAllPlateHidden();
	for (size_t i = 0; i < Index; i++)
	{
		DirtyPlateComponents[i]->SetHiddenInGame(false);
		DirtyPlateComponents[i]->SetVisibility(true);
	}
}

void ASinkTable::SetAllPlateHidden()
{
	for (int32 i = 0; i < DirtyPlateComponents.Num(); i++)
	{
		DirtyPlateComponents[i]->SetVisibility(false);
		DirtyPlateComponents[i]->SetHiddenInGame(true);
	}
}

void ASinkTable::AddDirtyPlateNum_Implementation(int Value)
{
	DirtyPlateNum += Value;
	if (4 < DirtyPlateNum)
	{
		DirtyPlateNum = 4;
	}
	else if (0 > DirtyPlateNum)
	{
		DirtyPlateNum = 0;
	}
}

void ASinkTable::AddCleanPlateNum_Implementation(int Value)
{
	CleanPlateNum += Value;

	if (4 < CleanPlateNum)
	{
		CleanPlateNum = 4;
	}
	else if (0 > CleanPlateNum)
	{
		CleanPlateNum = 0;
	}
}

void ASinkTable::SetCleanPlateMesh_Implementation()
{
	switch (CleanPlateNum)
	{
	case 0:
	{
		CleanPlateMeshComponent->SetStaticMesh(nullptr);
		break;
	}
	case 1:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("SinglePlate"));
		CleanPlateMeshComponent->SetStaticMesh(NewMesh);

		CleanPlateMeshComponent->SetRelativeLocation(FVector(0, 10, 0));
		break;
	}
	case 2:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("DoublePlate"));
		CleanPlateMeshComponent->SetStaticMesh(NewMesh);

		CleanPlateMeshComponent->SetRelativeLocation(FVector(0, 10, 20));
		break;
	}
	case 3:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("TriplePlate"));
		CleanPlateMeshComponent->SetStaticMesh(NewMesh);

		CleanPlateMeshComponent->SetRelativeLocation(FVector(0, 10, 0));
		break;
	}
	case 4:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("FullPlate"));
		CleanPlateMeshComponent->SetStaticMesh(NewMesh);

		CleanPlateMeshComponent->SetRelativeLocation(FVector(0, 10, 60));
		break;
	}
	default:
		break;
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
	DOREPLIFETIME(ASinkTable, CleanPlateMeshComponent);
	//DOREPLIFETIME(ASinkTable, bIsFirstPlateWashed);
	//DOREPLIFETIME(ASinkTable, bCallGetMoveFunction);

}