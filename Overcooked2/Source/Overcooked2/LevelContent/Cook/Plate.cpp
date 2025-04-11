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
#include <LevelContent/Table/NonTable/SinkTable.h>
#include <Global/State/GameState/CookingGameState.h>
#include <Global/OC2Global.h>

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

	// ?
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
	FindSinkTable();
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

void APlate::FindSinkTable()
{
	for (TActorIterator<ACookingTable> It(GetWorld()); It; ++It)
	{
		ACookingTable* PrepTableActor = *It;
		if (PrepTableActor->Tags.Contains("SinkTable"))
		{
			SinkTable = Cast<ASinkTable>(PrepTableActor);
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

void APlate::StackPlate(APlate* Plate)
{
	bIsCombinationSuccessful = false;

	if (PlateState == EPlateState::EMPTY || PlateState == EPlateState::DIRTY)
	{
		if (PlateState == Plate->PlateState) // 동일한 상태인 녀석만 쌓을 수 있다.
		{
			bIsCombinationSuccessful = true;
			// 1. 쌓인 접시와 나 자신(1)을 더한다.
			int PlateCount = Plate->GetPlateStackCount();
			AddPlateStackCount(PlateCount + 1);

			// 2. 스테이지에서 Plate를 제외시킨다.
			UOC2Global::MovePlate(GetWorld(), Plate);
			ChangePlateMesh();
		}
	}
}

void APlate::ChangePlateMesh()
{
	if (nullptr != GetWorld()->GetAuthGameMode())
	{
 		int a = 0;
	}
	if (nullptr == GetWorld()->GetAuthGameMode())
	{
		int a = 0;
	}
	int Count = GetPlateStackCount();

	switch (Count)
	{
	case 0:
	{
		ChangePlateMeshAndStatus(EPlateStackStatus::SINGLE, TEXT("SinglePlate"));
		break;
	}
	case 1:
	{
		ChangePlateMeshAndStatus(EPlateStackStatus::DOUBLE, TEXT("DoublePlate"));
		break;
	}
	case 2:
	{
		ChangePlateMeshAndStatus(EPlateStackStatus::TRIPLE, TEXT("TriplePlate"));
		break;
	}
	case 3:
	{
		ChangePlateMeshAndStatus(EPlateStackStatus::FULL, TEXT("FullPlate"));
		break;
	}
	default:
		break;
	}
}

void APlate::ChangePlateMeshAndStatus_Implementation(EPlateStackStatus Status, FName Name)
{
	PlateStackStatus = Status;
	
	UStaticMesh* NewStaticMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), Name);
	StaticMeshComponent->SetStaticMesh(NewStaticMesh);
}

void APlate::ForwardCookingTable(ACookingTable* Table)
{
	CookingTable = Table;
}

void APlate::Multicast_MovePlate_Implementation()
{
	SetActorLocation(UOC2Const::PlateSubmitLocation);
	CleanPlate();
	SetPlateState(EPlateState::EMPTY);

	ACookingGameState* GameState = Cast<ACookingGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (nullptr != GameState)
	{
		GameState->AddPlate(this);
	}
}

void APlate::SpawnWashPlate()
{
	if (nullptr != PlateSpawner)
	{
		PlateSpawner->PlaceItem(this);
		PlateSpawner->SetPlate(this);
	}
}

void APlate::Multicast_SpawnWashPlate_Implementation()
{
	SetActorLocation(UOC2Const::PlateSubmitLocation);
	CleanPlate();
	SetPlateState(EPlateState::EMPTY);
	PlateStackStatus = EPlateStackStatus::SINGLE; // 상태도 하나로 바꾸고
	ChangePlateMesh(); // 메시도 지우고

	ACookingGameState* GameState = Cast<ACookingGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (nullptr != GameState)
	{
		GameState->AddPlate(this);
	}

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&APlate::SpawnWashPlate,
		1.0f,   // 3초 뒤 실행
		false   // 반복 여부(false면 1회 실행)
	);
}

void APlate::RestorePlateToWorld()
{
	// 월드로 다시 편입시키고
	SetActorHiddenInGame(false); // 렌더
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void APlate::HiddenPlateToWorld()
{
	SetActorHiddenInGame(true);		// 렌더 끄고
	SetActorEnableCollision(false);	// 충돌 끄고
	SetActorTickEnabled(false);		// Tick 끄고
}

void APlate::SubmitPlate_Implementation()
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

void APlate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlate, Ingredients);
	DOREPLIFETIME(APlate, IngredientMesh);
	DOREPLIFETIME(APlate, PlateState);
	DOREPLIFETIME(APlate, bIsCombinationSuccessful);
	//DOREPLIFETIME(APlate, AnotherPlates);
	DOREPLIFETIME(APlate, PlateStackStatus);
	DOREPLIFETIME(APlate, CookingTable);
}