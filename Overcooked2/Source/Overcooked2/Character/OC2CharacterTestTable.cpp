// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2CharacterTestTable.h"
#include "Character/OC2Character.h"

// Sets default values
AOC2CharacterTestTable::AOC2CharacterTestTable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("MyStaticMesh");
	MySceneComponent= CreateDefaultSubobject<USceneComponent>("Fuck");
	Root = CreateDefaultSubobject<USceneComponent>("DefaultSceneComponent");
	RootComponent = Root;
	StaticMesh->SetupAttachment(RootComponent);
	MySceneComponent->SetupAttachment(RootComponent);

}

AIngredient* AOC2CharacterTestTable::SpawnIngredient(AActor* ChefActor)
{
	//UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Global/Data/DT_IngredientDataTable.DT_IngredientDataTable'_C"));

	//if (nullptr == DataTable)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IngredientDataTable을 찾지 못해 재료 스폰에 실패했습니다."));
	//	return nullptr;
	//}

	//FName RowName = *StaticEnum<EIngredientType>()->GetDisplayNameTextByValue(static_cast<int64>(Ingredient)).ToString();
	//FName RowName = *StaticEnum<EIngredientType>()->GetDisplayNameTextByValue(static_cast<int64>(Ingredient)).ToString();
	//FIngredientDataRow* IngredientData = DataTable->FindRow<FIngredientDataRow>(RowName, TEXT(""));

	//if (nullptr == IngredientData)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IngredinetDataTable에서 재료 타입과 일치하는 데이터를 추출에 실패했습니다."));
	//	return nullptr;
	//}

	FActorSpawnParameters SpawnParameters; // 적절한 오버로딩 함수 호출을 위해(회전값 추가), FActorSpawnParameters 사용
	FVector Location = FVector();
	if (nullptr != ChefActor)
	{
		Location = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	}
	FRotator Rotator = FRotator::ZeroRotator;

	// 1. 재료를 월드에 스폰한다.
	// Transform은 있지만 메시도 없는 빈 껍데기 상태
	AIngredient* NewIngredient = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), Location, Rotator, SpawnParameters);

	// 액터에 부착
	if (nullptr != ChefActor)
	{
		//NewIngredient->Interact(ChefActor);
	}

	if (nullptr == NewIngredient)
	{
		UE_LOG(LogTemp, Warning, TEXT("액터 스폰에 실패했습니다."));
		return nullptr;
	}

	// 2. 메시를 찾아서
	NewIngredient->Init(IngredientType);

	return NewIngredient;
}

void AOC2CharacterTestTable::PlaceItem(ACooking* Cook)
{
	CurCook = Cook;
	CurCook->AttachToComponent(MySceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

ACooking* AOC2CharacterTestTable::Interact(AActor* ChefActor)
{
	AOC2Character* Character = Cast<AOC2Character>(ChefActor);

	if (CurCook == nullptr)
	{
		if (Character->IsHolding())
		{
			return nullptr;
		}
		return SpawnIngredient(ChefActor);

	}
	else
	{
		ACooking* Ret = CurCook;
		CurCook = nullptr;
		return Ret;
	}
}



// Called when the game starts or when spawned
void AOC2CharacterTestTable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AOC2CharacterTestTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOC2CharacterTestTable::Highlight()
{
	StaticMesh->SetMaterial(0, HighlightMat);

}

void AOC2CharacterTestTable::OffHighlight()
{
	StaticMesh->SetMaterial(0, OriginMat);

}

