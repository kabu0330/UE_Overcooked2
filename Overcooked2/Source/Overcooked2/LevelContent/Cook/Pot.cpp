// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Pot.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Table/BurnerTable.h>
#include <Net/UnrealNetwork.h>

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_POT;

	SoupSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SoupSkeletalMeshComponent"));
	SoupSkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	SoupSkeletalMeshComponent->SetIsReplicated(true);


	FVector Pos = FVector(249, 1452, 60);
	StaticMeshComponent->SetRelativeLocation(Pos);
}

void APot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(APot, SoupSkeletalMeshComponent);
	DOREPLIFETIME(APot, SoupDynamicMaterial);
	DOREPLIFETIME(APot, NoneMaterial);
	DOREPLIFETIME(APot, PotState);
	DOREPLIFETIME(APot, PrevPotState);
	DOREPLIFETIME(APot, bIsRiceInPot);
	DOREPLIFETIME(APot, TimeElapsed);
	DOREPLIFETIME(APot, CookingTable);
	DOREPLIFETIME(APot, bIsBoiling);
}

UMaterialInstanceDynamic* APot::LoadNoneMaterial()
{
	const FString NoneMaterialName = TEXT("/Game/Resources/LevelResource/Object/Pot/M_NONE");
	UMaterial* NewMaterial = LoadObject<UMaterial>(nullptr, *NoneMaterialName);
	UMaterialInstanceDynamic* NewMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(NewMaterial, this);
	return NewMaterialInstanceDynamic;
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();

	NoneMaterial = LoadNoneMaterial(); // 여기서 해줘야 클라도  NULL 머티리얼 생성된다.
	SetSoupMaterial();

	//ChangeNoneMaterial();
}

void APot::SetSoupMaterial_Implementation()
{
	int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
	if (NumSoupMaterials == SoupDynamicMaterial.Num())
	{
		return;
	}
	for (int32 i = 0; i < NumSoupMaterials; i++)
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(i), this);
		SoupDynamicMaterial.Add(MaterialInstanceDynamic);
		SoupSkeletalMeshComponent->SetMaterial(i, MaterialInstanceDynamic);
	}
}

void APot::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);
	Cook(DeltaTime);
	ChangeState();
	//ChangeMaterialColor();
}

void APot::Cook(float DeltaTime)
{
	if (false == bIsBoiling)
	{
		return;
	}
	TimeElapsed += DeltaTime;
}

void APot::ChangeState_Implementation()
{
	if (!HasAuthority())
	{
		int a = 0;
	}
	// 상태가 변경되지 않았으면 리턴
	// Tick에서 들어오는데 하...
	//if (PrevPotState == PotState)
	//{
	//	return;
	//}
	PrevPotState = PotState;

	switch (PotState)
	{
	case EPotState::IDLE:
	{
		ChangeNoneMaterial();
		break;
	}
	case EPotState::HEATING:
	{
		int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
		for (int32 i = 0; i < NumSoupMaterials; i++)
		{
			SoupSkeletalMeshComponent->SetMaterial(i, SoupDynamicMaterial[i]);
		}

		break;
	}
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

void APot::ChangeNoneMaterial()
{
	if (!HasAuthority())
	{
		int a = 0;
	}
	int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
	for (int32 i = 0; i < NumSoupMaterials; i++)
	{
		SoupSkeletalMeshComponent->SetMaterial(i, NoneMaterial);
	}
}

void APot::ChangeMaterialColor()
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

void APot::Add_Implementation(AIngredient* Ingredient)
{
	if (true == bIsRiceInPot) // 1. 이미 쌀이 들어와있다면 리턴
	{
		return;
	}

	AIngredient* RawRice = Cast<AIngredient>(Ingredient);
	if (nullptr == RawRice) // 2. 지금 들어온 재료가 쌀이냐
	{
		return;
	}
	if (EIngredientState::EIS_BOILABLE != RawRice->GetCurIngredientState()) // 3. Boil 할 수 있고, 조리가 안 된 상태가 맞냐
	{
		return;
	}

	RawRice->RequestOC2ActorDestroy(); // 들어온 재료는 삭제
	
	bIsRiceInPot =  true;
	PotState = EPotState::HEATING;
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

	// 예외처리를 통과할 수 있는 재료는 쌀 뿐이므로 여기까지 오면 조리가 되지 않은 쌀이다.


	PotState = EPotState::BOILING;
	bIsBoiling = true;


	// 스켈레탈 메시에 조리 애니메이션 재생해야함

	return;
}

AIngredient* APot::GetCookedIngredient()
{
	return nullptr;
}

