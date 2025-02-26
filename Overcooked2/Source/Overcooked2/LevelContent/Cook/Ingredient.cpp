// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Ingredient.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIngredient::AIngredient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent; // 설정 안해주면 nullptr 나와서 터짐

	// Spawn되는 녀석의 기본 크기를 지정해줘야 한다면?
	//SetActorScale3D(FVector(100.0f, 100.0f, 100.0f));
}

void AIngredient::Init(FName Name)
{
	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// 1. 데이터 테이블을 가져온다.
	UDataTable* IngredientDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable"));

	if (nullptr == IngredientDataTable)
	{
		int a = 0;
	}
	
	FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(Name, TEXT(""));
	if (nullptr == IngredientData)
	{
		int a = 0;
	}

	//DataTable = UGlobalDataTable::GetActorClass(Name);
	//DataTable = 
	//StaticMeshComponent->SetStaticMesh(DataTable.BaseMesh);
	StaticMeshComponent->SetStaticMesh(IngredientData->BaseMesh);
	CurIngredientState = EIngredientState::EIS_NONE;
}

const FIngredientCookDataRow& AIngredient::CheckState(EIngredientState State)
{
	const FIngredientCookDataRow* Result = nullptr;

	for (size_t i = 0; i < DataTable.StateRows.Num(); i++)
	{
		if (DataTable.StateRows[i].IngredientState == State)
		{
			Result = &DataTable.StateRows[i];
			break;
		}
	}

	return *Result;
}

void AIngredient::ChageState(EIngredientState State)
{
	const FIngredientCookDataRow* Data = &CheckState(State);

	if (nullptr == Data)
	{
		return;
	}

	CurIngredientState = State;
	StaticMeshComponent->SetStaticMesh(Data->CookMesh);
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

