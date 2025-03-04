// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Ingredient.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIngredient::AIngredient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CookingType = ECookingType::ECT_INGREDIENT;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent; // 설정 안해주면 nullptr 나와서 터짐
}

// Called when the game starts or when spawned
void AIngredient::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AIngredient* AIngredient::Init(EIngredientType Type)
{
	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FName Name = GameInst->GetIngredientDataTableRowName(Type);
	
	// 1. 해당 재료 타입의 데이터 행 추출
	// FIngredientDataRow IngredientDataTable
	IngredientDataTable = &GameInst->GetIngredientDataRow(Name);
	if (nullptr == IngredientDataTable)
	{
		return nullptr;
	}

	// 2. Setting
	StaticMeshComponent->SetStaticMesh(IngredientDataTable->BaseMesh);
	IngredientType = IngredientDataTable->IngredientType;
	CurIngredientState = IngredientDataTable->StateRows[0].PrevIngredientState;

	// 3. Offset
	FVector Location = IngredientDataTable->Location;
	FRotator Rotation = IngredientDataTable->Rotation;
	Offset(Location, Rotation);

	return this;
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

AIngredient* AIngredient::ChangeState(EIngredientState State)
{
	const FIngredientCookDataRow* CookData = &CheckState(State);

	if (nullptr == CookData)
	{
		return nullptr;
	}

	DeactivateHighlight();

	CurIngredientState = State;
	StaticMeshComponent->SetStaticMesh(CookData->CookMesh);

	ActivateHighlight();

	FVector Location = CookData->Location;
	FRotator Rotation = CookData->Rotation;
	Offset(Location, Rotation);

	return this;
}

void AIngredient::DeactivateHighlight()
{
	if (bIsHighlighted)
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
	if (bIsHighlighted)
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
