// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Pot.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Table/BurnerTable.h>

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_POT;

	SoupSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SoupSkeletalMeshComponent"));
	SoupSkeletalMeshComponent->SetupAttachment(StaticMeshComponent);

	FVector Pos = FVector(249, 1452, 60);
	StaticMeshComponent->SetRelativeLocation(Pos);
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();

}

void APot::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);
	Cook(DeltaTime);
	ChangeAnimation();
	ChangeMaterialColor();
}

void APot::Cook(float DeltaTime)
{
	if (false == bIsBoiling)
	{
		return;
	}
	TimeElapsed += DeltaTime;
}

void APot::ChangeAnimation()
{
	if (nullptr == Ingredient)
	{
		return;
	}

	switch (PotState)
	{
	case EPotState::IDLE:
		break;
	case EPotState::BOILING:
		break;
	case EPotState::COOKED:
		break;
	case EPotState::OVERCOOKED:
		break;
	default:
		break;
	}
}

void APot::ChangeMaterialColor()
{
}

// Pot은 조리를 할 모든 요건이 갖춰졌는가?
bool APot::CanCook()
{

	if (nullptr == Ingredient) // 2. 재료가 있냐
	{
		return false;
	}

	return true;
}

void APot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

void APot::ForwardCookingTable(ACookingTable* Table)
{
	CookingTable = Table;
}

void APot::ForwardAttachToChef()
{
	CookingTable = nullptr;
}

void APot::SetBoil(ACooking* Rice)
{
	if (nullptr == CookingTable)
	{
		return;
	}
	ABurnerTable* BurnerTable =  Cast<ABurnerTable>(CookingTable);
	if (nullptr == BurnerTable) // 1. 버너 위에 있냐
	{
		return; // 가장 먼저 Ingredient를 넣는 함수이므로 CanCook 함수로 체크하지 않는다.
	}

	AIngredient* RawRice = Cast<AIngredient>(Rice);
	if (nullptr == RawRice) // 2. 재료가 쌀이냐
	{
		return;
	}
	if (EIngredientState::EIS_BOILABLE != RawRice->GetCurIngredientState()) // 3. Boil 할 수 있고, 조리가 안 된 상태가 맞냐
	{
		return;
	}

	// 예외처리를 통과할 수 있는 재료는 쌀 뿐이므로 여기까지 오면 조리가 되지 않은 쌀이다.

	Ingredient = RawRice;
	PotState = EPotState::BOILING;

	bIsBoiling = true;


	// 스켈레탈 메시에 조리 애니메이션 재생해야함

	return;
}

AIngredient* APot::GetCookedIngredient()
{
	Ingredient->ChangeState(EIngredientState::EIS_BOILED);

	AIngredient* CookedIngredient = Ingredient;
	Ingredient = nullptr;

	return CookedIngredient;
}

