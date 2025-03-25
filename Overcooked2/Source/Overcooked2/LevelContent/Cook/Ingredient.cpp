// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Ingredient.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/BillboardComponent.h"  
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AIngredient::AIngredient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_INGREDIENT;

	TextureBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("TextureBillboard"));
	TextureBillboard->bHiddenInGame = false;
	TextureBillboard->bIsScreenSizeScaled = true;
	TextureBillboard->bReceivesDecals = false;

	TextureBillboard->SetUsingAbsoluteRotation(true);
	TextureBillboard->SetUsingAbsoluteLocation(true);
	
	TextureBillboard->SetupAttachment(RootComponent);
	
	//StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AIngredient::OnOverlapBegin);
}

void AIngredient::SetType_Implementation(EIngredientType Type)
{
	// 3. SpawnActorDeferred 이후 BeginPlay 실행 이전 멤버변수는 변경
	// Replicated이기 때문에, 서버에서 변경하면 클라도 다 같이 바뀐다.
	IngredientType = Type;
}


void AIngredient::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	TArray<FName> OtherComponentTags = OtherComponent->ComponentTags;
 	if (nullptr != OtherActor && true == OtherComponent->ComponentHasTag(TEXT("Floor")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("재료와 바닥 충돌"));
		//StaticMeshComponent->BodyInstance.bLockXRotation = true;
		StaticMeshComponent->BodyInstance.bLockYRotation = true;
		StaticMeshComponent->BodyInstance.bLockZRotation = true;
	}

	StaticMeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
}


// Called when the game starts or when spawned
void AIngredient::BeginPlay()
{
	ACooking::BeginPlay();

	// 5. 서버에서 Spawn이 되면서 Init 함수 호출
	Init(IngredientType);

}

// Called every frame
void AIngredient::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

	if (nullptr != TextureBillboard)
	{
		FRotator FixedRotation = FRotator(0, 0, 0);
		TextureBillboard->SetWorldRotation(FixedRotation);

		FVector OffsetPos = FVector(0, 0, 100);
		TextureBillboard->SetWorldLocation(GetActorLocation() + OffsetPos);
	}
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

	// 2). 메시, 타입, 상태 설정
	SetIngredientData(GameInst, Name);

	// 3). Offset
	SetLocalOffset();


	// 4). 텍스처 설정, 회전값만 Tick에서 무시하도록 적용
	SetTexture();


	// Network Debug
	if (true == HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 스폰 성공"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 복제 성공"));
	}

	return this;
}


void AIngredient::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredient, IngredientType);
	DOREPLIFETIME(AIngredient, CurIngredientState);
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
	if (nullptr != Texture)
	{
		TextureBillboard->SetSprite(Texture);
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
