// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Pot.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Table/BurnerTable.h>
#include <Net/UnrealNetwork.h>
#include "GameFramework/Character.h"
#include <Global/GameMode/OC2GameMode.h>
#include "Components/BillboardComponent.h"  
#include <Global/Component/TimeEventComponent.h>

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_POT;

	SoupSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SoupSkeletalMeshComponent"));
	SoupSkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	SoupSkeletalMeshComponent->SetIsReplicated(true);

	InitTexture();

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent "));

	//FVector Pos = FVector(249, 1452, 60);
	//StaticMeshComponent->SetRelativeLocation(Pos);
}

void APot::InitTexture()
{
	TextureBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("TextureBillboard"));
	TextureBillboard->bHiddenInGame = false;
	TextureBillboard->bIsScreenSizeScaled = true;
	TextureBillboard->bReceivesDecals = false;

	FVector Scale(1.0f, 1.0f, 1.0f);
	TextureBillboard->SetRelativeScale3D(Scale);

	TextureBillboard->SetUsingAbsoluteRotation(true);
	TextureBillboard->SetUsingAbsoluteLocation(true);
}

void APot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APot, SoupSkeletalMeshComponent);
	DOREPLIFETIME(APot, SoupDynamicMaterial);
	DOREPLIFETIME(APot, NoneMaterial);
	DOREPLIFETIME(APot, PotState);
	//DOREPLIFETIME(APot, PrevPotState);
	DOREPLIFETIME(APot, bIsRiceInPot);
	DOREPLIFETIME(APot, TimeElapsed);
	DOREPLIFETIME(APot, CookingTable);
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();

	NoneMaterial = LoadNoneMaterial(); // 여기서 해줘야 클라도  NULL 머티리얼 생성된다.
	SetSoupMaterial();

	ChangeNoneMaterial();
	SetWarningTexture();
}

void APot::SetWarningTexture()
{
	const FString WarningTextureName = TEXT("/Game/Resources/LevelResource/CookObject/Object/Pot/Texture/BurnWarning");
	UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, *WarningTextureName);
	if (nullptr != NewTexture)
	{
		TextureBillboard->SetSprite(NewTexture);
		TextureBillboard->SetVisibility(false);
	}
}

UMaterialInstanceDynamic* APot::LoadNoneMaterial()
{
	const FString NoneMaterialName = TEXT("/Game/Resources/LevelResource/CookObject/Object/Pot/Soup/M_NONE");
	UMaterial* NewMaterial = LoadObject<UMaterial>(nullptr, *NoneMaterialName);
	UMaterialInstanceDynamic* NewMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(NewMaterial, this);
	return NewMaterialInstanceDynamic;
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
		UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(SoupSkeletalMeshComponent->GetMaterial(i), this);
		SoupDynamicMaterial.Add(MaterialInstanceDynamic);
		SoupSkeletalMeshComponent->SetMaterial(i, MaterialInstanceDynamic);
	}
}

void APot::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

	Cook(DeltaTime);
	SetAction();
	SetWarnigTextureOffset();
	BlinkTexture(DeltaTime);
}

void APot::SetWarnigTextureOffset()
{
	if (nullptr != TextureBillboard)
	{
		FRotator FixedRotation = FRotator(0, 0, 0);
		TextureBillboard->SetWorldRotation(FixedRotation);

		FVector OffsetPos = FVector(20, -180, 100);
		TextureBillboard->SetWorldLocation(GetActorLocation() + OffsetPos);
	}
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

	bIsRiceInPot = true;
	bIsCombinationSuccessful = true;
	PotState = EPotState::HEATING;
}

// 조리 시간 타이머 트리거
bool APot::IsBoiling()
{
	if (false == bIsRiceInPot) // 1. 냄비에 쌀이 있냐
	{
		return false;
	}
	//if (nullptr == CookingTable)
	//{
	//	return false;
	//}
	//ABurnerTable* BurnerTable = Cast<ABurnerTable>(CookingTable);
	//if (nullptr == BurnerTable) // 2. 버너 위에 있냐
	//{
	//	return false;
	//}
	return true;
}

void APot::Cook(float DeltaTime)
{
	if (false == IsBoiling())
	{
		return;
	}

	TimeElapsed += DeltaTime;

	float TimeToBoil = 4.0f;
	float TimeToCook = 12.0f;
	float TimeToWarning = 16.0f;
	float TimeToDanger = 20.0f;
	float TimeToScorch = 24.0f;
	float TimeToOvercook = 28.0f;

	if (EPotState::HEATING == PotState && 
		TimeToBoil < TimeElapsed)
	{
		PotState = EPotState::BOILING;
	}
	else if (EPotState::BOILING == PotState && 
		TimeToCook < TimeElapsed)
	{
		PotState = EPotState::COOKED;
	}
	else if (EPotState::COOKED == PotState && 
		TimeToWarning < TimeElapsed)
	{
		PotState = EPotState::COOKED_WARNING;
	}
	else if (EPotState::COOKED_WARNING == PotState && 
		TimeToDanger < TimeElapsed)
	{
		PotState = EPotState::COOKED_DANGER;
	}
	else if (EPotState::COOKED_DANGER == PotState && 
		TimeToScorch < TimeElapsed)
	{
		PotState = EPotState::SCORCHING;
	}
	else if (EPotState::SCORCHING == PotState && 
		TimeToOvercook < TimeElapsed)
	{
		PotState = EPotState::OVERCOOKED;
	}
}

void APot::SetAction_Implementation()
{
	if (!HasAuthority())
	{
		int a = 0;
	}

	// 상태가 변경되지 않았으면 리턴
	if (PrevPotState == PotState)
	{
		return;
	}
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
	{
	}
		break;
	case EPotState::COOKED_WARNING:
		bCanBlink = true;
		TextureBillboard->SetVisibility(true);
		BlinkTime = 1.0f;
		break;
	case EPotState::COOKED_DANGER:
		BlinkTime = 0.5f;
		break;
	case EPotState::SCORCHING:
		BlinkTime = 0.2f;
		break;
	case EPotState::OVERCOOKED:
		bCanBlink = false;
		TextureBillboard->SetVisibility(false);
		break;
	default:
		break;
	}
}

void APot::ChangeNoneMaterial()
{
	int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
	for (int32 i = 0; i < NumSoupMaterials; i++)
	{
		SoupSkeletalMeshComponent->SetMaterial(i, NoneMaterial);
	}
}

void APot::ChangeMaterialColor(FVector4 Color)
{
	int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
	for (int32 i = 1; i < NumSoupMaterials; i++)
	{
		SoupDynamicMaterial[i]->SetVectorParameterValue(FName("Tint"), Color);
		SoupSkeletalMeshComponent->SetMaterial(i, SoupDynamicMaterial[i]);
	}
}

void APot::ResetPot()
{
	PrevPotState = EPotState::MAX;
	PotState = EPotState::IDLE;
	bIsRiceInPot = false;
	TimeElapsed = 0.0f;
	bCanBlink = false;
	bIsCombinationSuccessful = false;

	ChangeNoneMaterial();
}

void APot::BlinkTexture(float DeltaTime)
{
	if (false == bCanBlink)
	{
		return;
	}


	if (true == TextureBillboard->IsVisible())
	{
		BlinkTimeElapsed += DeltaTime;
	}

	if (BlinkTime <= BlinkTimeElapsed && true == TextureBillboard->IsVisible())
	{
		TextureBillboard->SetVisibility(false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("텍스처 끔"));
		BlinkTimeElapsed = 0.0f;
		TimeEventComponent->AddEndEvent(BlinkTime, [this]()
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("텍스처 켬"));
				TextureBillboard->SetVisibility(true);
			});
	}
}

AIngredient* APot::GetRice()
{
	AGameModeBase* GameModeBase =  GetWorld()->GetAuthGameMode();
	if (nullptr == GameModeBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr == GameModeBase"));
		return nullptr;
	}

	AOC2GameMode* GameMode = Cast<AOC2GameMode>(GameModeBase);
	if (nullptr == GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr == GameMode"));
		return nullptr;
	}

	AIngredient* Rice = GameMode->SpawnIngredientActor(EIngredientType::EIT_RICE);
	Rice->ChangeState(EIngredientState::EIS_BOILED);

	ResetPot();

	return Rice;
}

// Table이 SetCookingTable 함수를 호출해주면 여기서 세팅
void APot::ForwardCookingTable(ACookingTable* Table)
{
	CookingTable = Table;
}

// 캐릭터가 냄비를 들어올렸을 때, 호출
void APot::ForwardAttachToChef()
{
	CookingTable = nullptr;
}