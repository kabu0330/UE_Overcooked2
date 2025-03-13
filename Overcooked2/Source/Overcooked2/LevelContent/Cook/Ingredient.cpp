// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Ingredient.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AIngredient::AIngredient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//SetReplicates(true);
	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_INGREDIENT;

}

void AIngredient::SetType_Implementation(EIngredientType Type)
{
	// 3. SpawnActorDeferred 이후 BeginPlay 실행 이전 멤버변수는 변경
	// Replicated이기 때문에, 서버에서 변경하면 클라도 다 같이 바뀐다.
	IngredientType = Type;
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

	// 2). Setting
	StaticMeshComponent->SetStaticMesh(GameInst->GetIngredientStaticMesh(Name.ToString()));
	//StaticMeshComponent->SetStaticMesh(IngredientDataTable->BaseMesh);
	IngredientType = IngredientDataTable->IngredientType;
	CurIngredientState = IngredientDataTable->StateRows[0].PrevIngredientState;

	// 3). Offset
	FVector Location = IngredientDataTable->Location;
	FRotator Rotation = IngredientDataTable->Rotation;
	Offset(Location, Rotation);


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

void AIngredient::Offset(FVector Pos, FRotator Rot)
{
	AddActorLocalOffset(Pos);
	SetActorRelativeRotation(Rot);
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
	//StaticMeshComponent->SetStaticMesh(GameInst->GetIngredientStaticMesh(Name.ToString()));
	StaticMeshComponent->SetStaticMesh(CookData->CookMesh);

	ActivateHighlight();

	FVector Location = CookData->Location;
	FRotator Rotation = CookData->Rotation;
	Offset(Location, Rotation);

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
