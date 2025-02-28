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

// 데이터 테이블에서 적절한 메시를 찾고, 재료의 상태를 초기화하는 함수
void AIngredient::Init(FName Name)
{
	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// 1. 데이터 테이블을 가져온다.
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable"));

	if (nullptr == DataTable)
	{
		int a = 0;
	}

	// 2. 행 이름(Fish)을 기준으로 데이터 열("생선", 메시경로, State)을 가져온다.
	FIngredientDataRow* IngredientData = DataTable->FindRow<FIngredientDataRow>(Name, TEXT(""));
	IngredientDataTable = *IngredientData;
	if (nullptr == IngredientData)
	{
		int a = 0;
	}

	//DataTable = UGlobalDataTable::GetActorClass(Name);
	//DataTable = 
	//StaticMeshComponent->SetStaticMesh(DataTable.BaseMesh);

	// 3. 추출한 데이터 열 값을 넣어준다.
	StaticMeshComponent->SetStaticMesh(IngredientData->BaseMesh);
	IngredientType = IngredientData->IngredientType;
	CurIngredientState = IngredientData->StateRows[0].PrevIngredientState; 
	// 위치값과 회전값을 데이터 테이블에서 받아서 저장
	//FVector Pos = ;
	//Offset()
}

//void AIngredient::Init(EIngredientType Type)
//{
//	//UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//
//	//// 1. 데이터 테이블을 가져온다.
//	//UDataTable* IngredientDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable"));
//
//	////DataTable = GameInst->
//	//FName RowName = GameInst->GetIngredientDataTableRowName(Type);
//
//	//if (nullptr == IngredientDataTable)
//	//{
//	//	int a = 0;
//	//}
//
//	//// 2. 행 이름(Fish)을 기준으로 데이터 열("생선", 메시경로, State)을 가져온다.
//	////FIngredientDataRow* IngredientData = IngredientDataTable->FindRow<FIngredientDataRow>(Name, TEXT(""));
//	////if (nullptr == IngredientData)
//	////{
//	////	int a = 0;
//	////}
//
//	////DataTable = UGlobalDataTable::GetActorClass(Name);
//	////DataTable = 
//	////StaticMeshComponent->SetStaticMesh(DataTable.BaseMesh);
//
//	//// 3. 추출한 데이터 열 값을 넣어준다.
//	//StaticMeshComponent->SetStaticMesh(IngredientData->BaseMesh);
//	//IngredientType = IngredientData->IngredientType;
//	//CurIngredientState = IngredientData->StateRows[0].PrevIngredientState; // 이게 맞나???
//}

const FIngredientCookDataRow& AIngredient::CheckState(EIngredientState State)
{
	const FIngredientCookDataRow* Result = nullptr;

	for (size_t i = 0; i < IngredientDataTable.StateRows.Num(); i++)
	{
		if (IngredientDataTable.StateRows[i].IngredientState == State)
		{
			Result = &IngredientDataTable.StateRows[i];
			break;
		}
	}

	return *Result;
}

void AIngredient::Offset(FVector Pos, FRotator Rot)
{
	FVector Scale = FVector(0.7f, 0.7f, 0.7f);
	SetActorRelativeScale3D(Scale);
}

void AIngredient::ChangeState(EIngredientState State)
{
	const FIngredientCookDataRow* Data = &CheckState(State);

	if (nullptr == Data)
	{
		return;
	}

	CurIngredientState = State;
	StaticMeshComponent->SetStaticMesh(Data->CookMesh);
}



