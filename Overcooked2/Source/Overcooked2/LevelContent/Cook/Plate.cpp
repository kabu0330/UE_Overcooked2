// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Plate.h"
#include <LevelContent/Cook/Ingredient.h>
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include <Global/Data/OC2GlobalData.h>
#include "Components/BillboardComponent.h"  
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/PlateIconWidget.h>
#include "LevelContent/Table/CookingTable.h"
#include "LevelContent/Table/NonTable/PlateSpawner.h"
#include "EngineUtils.h"

// Sets default values
APlate::APlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CookingType = ECookingType::ECT_PLATE;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientMesh"));


	FVector Scale = FVector(2.0f, 2.0f, 2.0f);
	StaticMeshComponent->SetRelativeScale3D(Scale);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

}

void APlate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	IngredientMesh->SetIsReplicated(true); // 컴포넌트 네트워크 동기화
}

void APlate::Multicast_SubmitPlate_Implementation()
{
	SetActorLocation(UOC2Const::PlateSubmitLocation);
	CleanPlate();
	SetPlateState(EPlateState::DIRTY);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&APlate::SpawnPlate,
		3.0f,   // 3초 뒤 실행
		false   // 반복 여부(false면 1회 실행)
	);
}

void APlate::SpawnPlate()
{
	if (nullptr != PlateSpawner)
	{
		PlateSpawner->PlaceItem(this);
		PlateSpawner->SetPlate(this);
	}
}

// Called when the game starts or when spawned
void APlate::BeginPlay()
{
	ACooking::BeginPlay();
	
	InitWidgetComponent();

	FindPlateSpawner();

	// Debug
}

void APlate::InitWidgetComponent()
{
	// 위젯 클래스 지정
	WidgetComponent->SetWidgetClass(SubclassWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		IconWidget = Cast<UPlateIconWidget>(UserWidget);
	}

	WidgetComponent->SetDrawAtDesiredSize(true);   // 위젯의 실제 크기로 렌더
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f)); // 중심 정렬
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 월드 공간에서 3D처럼 보이게
	WidgetComponent->bHiddenInGame = false;

	// 카메라를 향하도록 설정
	WidgetComponent->SetTwoSided(true);
	WidgetComponent->SetTickWhenOffscreen(true);

	IconWidget->Init();
}

void APlate::FindPlateSpawner()
{
	// TActorIterator를 사용하여 월드 내 모든 APrepTable 액터를 순회

	for (TActorIterator<ACookingTable> It(GetWorld()); It; ++It)
	{
		ACookingTable* PrepTableActor = *It;
		if (PrepTableActor->Tags.Contains("PlateSpawner"))
		{
			PlateSpawner = Cast<APlateSpawner>(PrepTableActor);
		}
	}
}

// Called every frame
void APlate::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

}

bool APlate::IsDirtyPlate()
{
	if (nullptr == StaticMeshComponent)
	{
		return false;
	}

	return PlateState == EPlateState::DIRTY;
}

void APlate::WashPlate_Implementation()
{
	if (true == IsDirtyPlate())
	{
		PlateState = EPlateState::EMPTY;
		SetMesh();
	}
}

void APlate::SetPlateState_Implementation(EPlateState State)
{
	PlateState = State;
	SetMesh();
}

void APlate::CleanPlate_Implementation()
{
	PlateState = EPlateState::EMPTY;
	IngredientMesh->SetStaticMesh(nullptr);
	Ingredients.Empty();
	IconWidget->Reset();
	bIsCombinationSuccessful = false;
}

void APlate::SetMesh()
{
	UTexture* Texture = nullptr;
	if (EPlateState::DIRTY == PlateState)
	{
		Texture = DirtyTexture;
	}
	else
	{
		Texture = CleanTexture;
	}
	SetMaterialTexture(Texture);
}

void APlate::SetMaterialTexture(UTexture* Texture)
{
	// 1. 스태틱 메시의 머티리얼을 바꿀건데

	// 2. 이미 동적으로 생성한 머티리얼 인스턴스 다이나믹이 존재하면 
	UMaterialInstanceDynamic* MaterialInstanceDynamic = Cast<UMaterialInstanceDynamic>(StaticMeshComponent->GetMaterial(0));
	if (nullptr != MaterialInstanceDynamic)
	{
		// 3. 기존 머티리얼 인스턴스 다이나믹을 그대로 사용하고
		MaterialInstanceDynamic->SetTextureParameterValue(FName(TEXT("DiffuseColorMap")), Texture);
		StaticMeshComponent->SetMaterial(0, MaterialInstanceDynamic);
		return;
	}

	// 4. 기존에 만들어진 머티리얼 인스턴스 다이나믹이 없다면 == SetMaterial이 처음이라면
	//UMaterialInterface* Material = StaticMeshComponent->GetMaterial(0);
	//if (nullptr != Material)
	//{
	//	// 5. 이제 기존의 머티리얼은 안쓰고 머티리얼 인스턴스 다이나믹을 만들어서 쓸 것이다.
	//	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	//	if (nullptr != DynamicMaterial)
	//	{
	//		// 6. 바꿀 텍스처를 에디터에서 설정해줬다면 그걸로 바꿔라.
	//		if (nullptr != Texture)
	//		{
	//			DynamicMaterial->SetTextureParameterValue(FName(TEXT("DiffuseAdd")), Texture);
	//			StaticMeshComponent->SetMaterial(0, DynamicMaterial);
	//		}
	//	}
	//}
}

void APlate::ForwardAttachToChef()
{
	FVector Offset = FVector(90, 0, -40);
	StaticMeshComponent->SetRelativeLocation(Offset);
}

void APlate::ForwardDetachToChef()
{
}

bool APlate::CanPlaceOnPlate(AIngredient* Ingredient)
{
	if (nullptr == Ingredient)
	{
		return false;
	}
	if (ECookingType::ECT_INGREDIENT != Ingredient->GetCookingType())
	{
		return false;
	}
	if (EPlateState::COMPLETED == PlateState || EPlateState::DIRTY == PlateState)
	{	// 이미 완성된 요리나 세척 전의 접시는 재료를 올릴 수 없다.
		return false;
	}
	if (EIngredientState::EIS_NONE == Ingredient->GetCurIngredientState())
	{	// 손질되지 않은 재료는 접시에 올릴 수 없다.
		return false;
	}
	if (nullptr == StaticMeshComponent)
	{
		return false;
	}
	return true;
}

void APlate::Add_Implementation(AIngredient* Ingredient)
{
	bIsCombinationSuccessful = false;

	if (false == CanPlaceOnPlate(Ingredient))
	{
		return;
	}

	// 1. 손질된 재료를 추가한다.
	FRecipe Recipe;
	Recipe.IngredientType = Ingredient->GetIngredientType();
	Recipe.IngredientState = Ingredient->GetCurIngredientState();
	Ingredients.Add(Recipe);

	// 2. RecipeDataTable과 비교하여 데이터 테이블에 해당 재료조합이 존재하는지 확인
	FPlateInitData InitData = UOC2GlobalData::GetPlateMesh(GetWorld(), Ingredients);

	// 3-1. 데이터를 획득하는데 실패했다면
	if (nullptr == InitData.StaticMesh) 
	{
		Ingredients.Pop(); // 재료 자료구조에서 제거하고 리턴
		return;
	}
	else // 3-2. 데이터를 획득하는데 성공했다면 
	{
		IngredientMesh->SetStaticMesh(InitData.StaticMesh); // 메시 변경
		PlateState = EPlateState::OCCUPIED;
		if (nullptr != IngredientMesh)
		{
			// 3-3. 접시 위에 올라갈 요리 메시 세팅
			SetIngredinetMesh(InitData);

			// 3-5. 기존에 존재하는 재료는 월드에서 삭제
			Ingredient->RequestOC2ActorDestroy();

			// 4. Texture 추가
			SetIngredinetTextures(InitData);
			return;
		}
	}
	return;
}

void APlate::SetIngredinetMesh(FPlateInitData Data)
{
	// 1. 물리 잠시 끄고
	SetSimulatePhysics(false); // 컴포넌트와 충돌로 날아가는 움직이는 것을 방지하기 위해 물리를 잠시 끈다.

	// 2. IngredientMesh의 충돌체와 물리를 끈다.
	IngredientMesh->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IngredientMesh->SetCollisionProfileName(TEXT("NoCollision"));
	IngredientMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IngredientMesh->SetSimulatePhysics(false);

	// 3. Offset
	IngredientMesh->SetRelativeLocation(Data.OffsetLocation);
	IngredientMesh->SetRelativeRotation(Data.OffsetRotation);
	IngredientMesh->SetRelativeScale3D(Data.OffsetScale);

	// 4. 물리 다시 켜고
	SetSimulatePhysics(true);
	bIsCombinationSuccessful = true;
}

void APlate::SetIngredinetTextures(FPlateInitData Data)
{
	TArray<UTexture2D*> Textures;
	for (size_t i = 0; i < Data.IngredientTextures.Num(); i++)
	{
		UTexture2D* Texture = Cast<UTexture2D>(Data.IngredientTextures[i]);
		if (nullptr != Texture)
		{
			Textures.Add(Texture);
		}
	}
	IconWidget->SetIngredientTextures(Textures);

}

void APlate::StackPlate_Implementation(APlate* Plate)
{
	if (PlateState == EPlateState::EMPTY || PlateState == EPlateState::DIRTY)
	{
		if (PlateState == Plate->PlateState) // 동일한 상태인 녀석만 쌓을 수 있다.
		{
			AnotherPlates.Add(Plate);
			
			// 이미 쌓여있는 접시를 합치는 거라면
			// Debug 필요. Plate->AnotherPlates[i]->AnotherPlates는 항상 0이라고 확신할 수 있을까?
			if (false == Plate->AnotherPlates.IsEmpty())
			{
				for (size_t i = 0; i < Plate->AnotherPlates.Num(); i++)
				{
					AnotherPlates.Add(Plate->AnotherPlates[i]);
				}
				Plate->AnotherPlates.Empty();
			}

			ChangePlateMesh();
		}
	}
}

void APlate::ChangePlateMesh()
{
	int Count = AnotherPlates.Num();

	HideAnotherPlates();

	switch (Count)
	{
	case 0:
	{
		StackUpPlate(EPlateStackStatus::SINGLE, TEXT("SinglePlate"));
		break;
	}
	case 1:
	{
		StackUpPlate(EPlateStackStatus::DOUBLE, TEXT("DoublePlate"));
		break;
	}
	case 2:
	{
		StackUpPlate(EPlateStackStatus::TRIPLE, TEXT("TriplePlate"));
		break;
	}
	case 3:
	{
		StackUpPlate(EPlateStackStatus::FULL, TEXT("FullPlate"));
		break;
	}
	default:
		break;
	}
}

void APlate::StackUpPlate(EPlateStackStatus Status, FName Name)
{
	PlateStackStatus = Status;

	UStaticMesh* NewStaticMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), Name);
	StaticMeshComponent->SetStaticMesh(NewStaticMesh);
}

APlate* APlate::TakeCleanPlate()
{
	int32 Count = AnotherPlates.Num();
	if (0 == Count) // 내가 가지고 있는 다른 접시가 없다면 내 포인터를 넘겨준다.
	{
		return this;
	}

	// 설거지된 접시를 찾아서 하나 꺼내준다.
	for (size_t i = 0; i < AnotherPlates.Num(); i++)
	{
		if (EPlateState::EMPTY == AnotherPlates[i]->PlateState)
		{
			// 꺼내 줄 때는 다시 숨김 설정을 해제한다.
			AnotherPlates[i]->SetActorHiddenInGame(false);
			AnotherPlates[i]->SetActorEnableCollision(true);
			AnotherPlates[i]->SetActorTickEnabled(true);

			APlate* AnotherPlate = AnotherPlates[i];
			AnotherPlates.RemoveAt(i);
			return AnotherPlate;
		}
	}

	return nullptr; // 꺼내줄 녀석이 없다.
	// 꺼낼 녀석이 없다면 this를 줘야 할까?
}

void APlate::HideAnotherPlates()
{
	// 접시 다 숨긴다.
	for (int i = 0; i < AnotherPlates.Num(); i++)
	{
		AnotherPlates[i]->SetActorHiddenInGame(true);		// 렌더 끄고
		AnotherPlates[i]->SetActorEnableCollision(false);	// 충돌 끄고
		AnotherPlates[i]->SetActorTickEnabled(false);		// Tick 끄고
	}
}

void APlate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlate, Ingredients);
	DOREPLIFETIME(APlate, IngredientMesh);
	DOREPLIFETIME(APlate, PlateState);
	DOREPLIFETIME(APlate, bIsCombinationSuccessful);
	DOREPLIFETIME(APlate, AnotherPlates);
	DOREPLIFETIME(APlate, PlateStackStatus);
}