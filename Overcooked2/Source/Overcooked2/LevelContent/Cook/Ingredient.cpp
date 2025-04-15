// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Ingredient.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/BillboardComponent.h"  
#include <LevelContent/Cook/Widget/IngredientIconWidget.h>
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "LevelContent/Cook/Pot.h"
#include "LevelContent/Table/CookingTable.h"
#include "LevelContent/Table/NonTable/SinkTable.h" 
#include "LevelContent/Table/BurnerTable.h"	 
#include "Character/OC2Character.h"

// Sets default values
AIngredient::AIngredient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_INGREDIENT;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
}

void AIngredient::SetType_Implementation(EIngredientType Type)
{
	// 3. SpawnActorDeferred 이후 BeginPlay 실행 이전 멤버변수는 변경
	// Replicated이기 때문에, 서버에서 변경하면 클라도 다 같이 바뀐다.
	IngredientType = Type;
}

void AIngredient::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	StaticMeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	CheckPlacement();
	bIsThrowing = false;
	ThrownTime = 0.0f;
}

// Called when the game starts or when spawned
void AIngredient::BeginPlay()
{
	ACooking::BeginPlay();

	// 5. 서버에서 Spawn이 되면서 Init 함수 호출
	InitIconWidget();
	Init(IngredientType);
}

void AIngredient::InitIconWidget()
{
	// 위젯 클래스 지정
	WidgetComponent->SetWidgetClass(SubclassWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		IconWidget = Cast<UIngredientIconWidget>(UserWidget);
	}

	WidgetComponent->SetDrawAtDesiredSize(true);   // 위젯의 실제 크기로 렌더
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f)); // 중심 정렬
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 월드 공간에서 3D처럼 보이게
	WidgetComponent->bHiddenInGame = false;

	// 카메라를 향하도록 설정
	WidgetComponent->SetTwoSided(true);
	WidgetComponent->SetTickWhenOffscreen(true);
}

AIngredient* AIngredient::Init(EIngredientType Type)
{
	// 6. 서버에서 한 번 호출, 클라에서도 각 한 번씩 호출
	// GameMode::SpawnActorDeferred -> AIngredinet::SetType -> GameMode::FinishSpawning -> AIngredinet::BeginPlay -> Init

	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FName Name = GameInst->GetIngredientDataTableRowName(Type);

	// 1). 해당 재료 타입의 데이터 행 추출
	// FIngredientDataRow IngredientDataTable
	IngredientDataTable = &GameInst->GetIngredientDataRow(Name);
	if (nullptr == IngredientDataTable)
	{
		return nullptr;
	}

	// 2-1). 메시, 타입, 상태 설정
	SetIngredientData(GameInst, Name);

	// 2-2). 머티리얼 
	CreateDynamicMaterial();

	// 3). Offset
	SetLocalOffset();


	// 4). 텍스처 설정, 회전값만 Tick에서 무시하도록 적용
	SetTexture();

	return this;
}

// Called every frame
void AIngredient::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

	if (bIsThrowing)
	{
		ThrownTime += DeltaTime;
	}
}

void AIngredient::SetIngredientData(UOC2GameInstance* Inst, FName Name)
{
	// 2). Setting
	StaticMeshComponent->SetStaticMesh(Inst->GetIngredientStaticMesh(Name.ToString()));
	IngredientType = IngredientDataTable->IngredientType;
	CurIngredientState = IngredientDataTable->StateRows[0].PrevIngredientState;
}

void AIngredient::SetLocalOffset()
{
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = IngredientDataTable->Rotation;
	FVector Scale = IngredientDataTable->Scale;
	Offset(Location, Rotation, Scale);
}

void AIngredient::SetTexture()
{
	UTexture2D* Texture = IngredientDataTable->IngredientTexture;
	IconWidget->SetTexture(Texture);
}

void AIngredient::CreateDynamicMaterial()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	if (true == MeshComponents.IsEmpty())
	{
		return;
	}

	int Count = 0;

	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (nullptr == Mesh)
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			UMaterialInterface* Material = Mesh->GetMaterials()[i];
			if (nullptr != Material)
			{
				UMaterialInstanceDynamic* CastMaterial = Cast<UMaterialInstanceDynamic>(Material);
				if (nullptr != CastMaterial) // 이미 다이나믹 머티리얼이 생성되어 있다면 또 만들지 않는다.
				{
					continue;
				}
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
				if (nullptr != DynamicMaterial)
				{
					float Temp;
					Mesh->GetMaterials()[i]->GetScalarParameterValue(FName("DiffuseAdd"), Temp);
					DiffuseColorMapWeights.Add(Temp);
					Mesh->SetMaterial(i, DynamicMaterial);
				}
			}
		}
	}
}

void AIngredient::Offset(FVector Pos, FRotator Rot, FVector Scale)
{
	SetActorRelativeLocation(Pos);
	SetActorRelativeRotation(Rot);
	SetActorRelativeScale3D(Scale);
}

bool AIngredient::IsCooked()
{
	switch (CurIngredientState)
	{
	case EIngredientState::EIS_FINISHED:
	case EIngredientState::EIS_CHOPPED:
	case EIngredientState::EIS_GRILLED:
	case EIngredientState::EIS_FRYABED:
	case EIngredientState::EIS_BOILED:
		return true;

	case EIngredientState::EIS_CHOPPABLE:
	case EIngredientState::EIS_GRILLABLE:
	case EIngredientState::EIS_FRYABLE:
	case EIngredientState::EIS_BOILABLE:
	case EIngredientState::EIS_NONE:
	case EIngredientState::EIS_OVERCOOKED:
	case EIngredientState::EIS_MAX:
		return false;

	default:
		break;
	}
	return false;
}

void AIngredient::CheckPlacement()
{
	FVector TraceLocation = GetActorLocation() + FVector(0, 0, -50); // SceneComponent 위치 가져오기

	// 감지할 오브젝트 유형 설정 (예: WorldDynamic)
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);

	// 트레이스 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신 무시

	// 결과 저장 배열
	TArray<FHitResult> HitResults;

	bool bHit = false;
	if (StaticMeshComponent->GetPhysicsLinearVelocity().Length() < 0.2f && ThrownTime > 0.1f)
	{
		// Sphere Trace 실행
		bHit = GetWorld()->SweepMultiByObjectType(
			HitResults,
			TraceLocation, TraceLocation,  // Start와 End 동일 (정지된 구)
			FQuat::Identity,
			ObjectQueryParams,
			FCollisionShape::MakeSphere(100.0f),
			QueryParams
		);
	}

	if (bHit)
	{
		HitResults.Sort([TraceLocation](const FHitResult& A, const FHitResult& B) {
			return FVector::DistSquared(TraceLocation, A.GetActor()->GetActorLocation()) < FVector::DistSquared(TraceLocation, B.GetActor()->GetActorLocation());
			});


		// maybe Interactable.
		ACookingTable* ClosestTable = nullptr;
		for (auto Result : HitResults)
		{
			if (Result.GetActor()->IsA<AOC2Character>())
			{
				AOC2Character* Chef = Cast<AOC2Character>(Result.GetActor());
				if (Chef != Thrower && false == Chef->IsHolding())
				{
					Chef->Grab(this);
					bIsThrowing = false;
					ThrownTime = 0.0f;
					break;
				}
			}
			else if (Result.GetActor()->IsA<ACookingTable>())
			{
				ClosestTable = Cast<ACookingTable>(Result.GetActor());
				break;
			}
		}
		if (ClosestTable != nullptr)
		{
			if (ClosestTable->IsUsing() == false && false == ClosestTable->IsA<ASinkTable>())
			{
				AttachToChef(ClosestTable);
				ClosestTable->PlaceItem(this);
			}
			else
			{
				APot* Pot = Cast<APot>(ClosestTable->GetCookingPtr());
				if (Pot != nullptr)
				{
					Pot->Add(this);
					ClosestTable->PlaceItem(Pot);
				}
			}
			bIsThrowing = false;
			ThrownTime = 0.0f;
		}
	}
}

const FIngredientCookDataRow& AIngredient::CheckState(EIngredientState State)
{
	const FIngredientCookDataRow* Result = nullptr;

	for (size_t i = 0; i < IngredientDataTable->StateRows.Num(); i++)
	{
		if (IngredientDataTable->StateRows[i].IngredientState == State)
		{
			Result = &IngredientDataTable->StateRows[i];
			break;
		}
	}

	return *Result;
}

void AIngredient::ChangeState_Implementation(EIngredientState State)
{
	const FIngredientCookDataRow* CookData = &CheckState(State);

	if (nullptr == CookData)
	{
		return;
	}

	DeactivateHighlight();

	CurIngredientState = State;
	StaticMeshComponent->SetStaticMesh(CookData->CookMesh);

	ActivateHighlight();

	FVector Location = CookData->Location;
	FRotator Rotation = CookData->Rotation;
	FVector Scale = FVector::OneVector;
	Offset(Location, Rotation, Scale);

	return;
}

void AIngredient::DeactivateHighlight()
{
	if (true == bIsHighlighted)
	{
		RestoreMaterial();
		for (int i = 0; i < StaticMeshComponent->GetNumMaterials(); i++)
		{
			StaticMeshComponent->SetMaterial(i, nullptr);
		}
		bIsHighlighted = true;
	}
}

void AIngredient::ActivateHighlight()
{
	if (true == bIsHighlighted)
	{
		ApplyMaterialHighlight();
	}
	else
	{
		for (int i = 0; i < StaticMeshComponent->GetNumMaterials(); i++)
		{
			StaticMeshComponent->SetMaterial(i, nullptr);
		}
	}
}

void AIngredient::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredient, IngredientType);
	DOREPLIFETIME(AIngredient, CurIngredientState);
	DOREPLIFETIME(AIngredient, Thrower);
	DOREPLIFETIME(AIngredient, bIsThrowing);
	DOREPLIFETIME(AIngredient, ThrownTime);
}