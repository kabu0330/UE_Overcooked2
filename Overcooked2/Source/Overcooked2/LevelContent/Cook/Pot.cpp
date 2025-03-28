// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Pot.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Table/BurnerTable.h>
#include <Net/UnrealNetwork.h>
#include "GameFramework/Character.h"
#include <Global/GameMode/OC2GameMode.h>
#include "Components/BillboardComponent.h"  
#include <Global/Component/TimeEventComponent.h>
#include <Global/Data/OC2GlobalData.h>

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

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent"));

	StaticMeshComponent->SetRelativeLocation(InitPos);
}

void APot::InitTexture()
{
	TextureBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("TextureBillboard"));
	TextureBillboard->bHiddenInGame = false;
	//TextureBillboard->bIsScreenSizeScaled = true;
	TextureBillboard->bReceivesDecals = false;

	FVector Scale(1.0f, 1.0f, 1.0f);
	TextureBillboard->SetRelativeScale3D(Scale);

	TextureBillboard->SetUsingAbsoluteRotation(true);
	TextureBillboard->SetUsingAbsoluteLocation(true);

	TextureBillboard->SetVisibility(false);
}

void APot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APot, SoupSkeletalMeshComponent);
	DOREPLIFETIME(APot, NoneMaterial);
	DOREPLIFETIME(APot, PotState);
	DOREPLIFETIME(APot, bIsCombinationSuccessful);
	DOREPLIFETIME(APot, bIsRiceInPot);
	DOREPLIFETIME(APot, bIsCooked);
	DOREPLIFETIME(APot, TimeElapsed);
	DOREPLIFETIME(APot, CookingTable);
	DOREPLIFETIME(APot, TextureBillboard);
	DOREPLIFETIME(APot, bIsCombinationSuccessful);
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();

	NoneMaterial = LoadNoneMaterial(); // 여기서 해줘야 클라도 NULL 머티리얼 생성된다.
	SetSoupMaterial();

	ChangeNoneMaterial();
}

UMaterialInstanceDynamic* APot::LoadNoneMaterial()
{
	const FString NoneMaterialName = TEXT("/Game/Resources/LevelResource/CookObject/Object/Pot/Soup/M_NONE");
	UMaterial* NewMaterial = LoadObject<UMaterial>(nullptr, *NoneMaterialName);
	UMaterialInstanceDynamic* NewMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(NewMaterial, this);
	return NewMaterialInstanceDynamic;
}

void APot::SetSoupMaterial()
{
	int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
	if (NumSoupMaterials == SoupDynamicMaterial.Num())
	{
		return;
	}
	for (int32 i = 0; i < NumSoupMaterials; i++)
	{
		//UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(SoupSkeletalMeshComponent->GetMaterial(i), this);
		UMaterialInstanceDynamic* MaterialInstanceDynamic = Cast<UMaterialInstanceDynamic>(SoupSkeletalMeshComponent->GetMaterials()[i]);
		if (nullptr != MaterialInstanceDynamic)
		{
			SoupDynamicMaterial.Add(MaterialInstanceDynamic);
			SoupSkeletalMeshComponent->SetMaterial(i, MaterialInstanceDynamic);
		}
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

void APot::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

	Cook(DeltaTime);
	SetAction();
	SetWarnigTextureOffset();
	BlinkTexture(DeltaTime);

	if (true == bColorChanging)
	{
		CurrentColor = FMath::Lerp(CurrentColor, TargetColor, DeltaTime * ColorChangeSpeed);

		// 실제 머티리얼에 적용
		FVector4 Color = FVector4(CurrentColor.R, CurrentColor.G, CurrentColor.B, CurrentColor.A);
		ChangeMaterialColor(Color);

		// 목표에 거의 도달했으면 종료
		if (FVector(CurrentColor - TargetColor).Size() < 0.01f)
		{
			CurrentColor = TargetColor;
			bColorChanging = false;
		}
	}
}

void APot::SetWarnigTextureOffset()
{
	if (nullptr != TextureBillboard)
	{
		FRotator FixedRotation = FRotator(0, 0, 0);
		TextureBillboard->SetWorldRotation(FixedRotation);

		FVector OffsetPos = FVector(50, -180, 100);
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

	// Debug
	float TimeToBoil = 1.0f;
	float TimeToCook = 4.0f;
	float TimeToWarning = 7.0f;
	float TimeToDanger = 10.0f;
	float TimeToScorch = 13.0f;
	float TimeToOvercook = 16.0f;

	// Real Time
	//float TimeToOvercook = 25.0f;

	if (TimeToOvercook < TimeElapsed) // 불필요한 계산 차단
	{
		return;
	}

	TimeElapsed += DeltaTime;

	//float TimeToBoil = 4.0f;
	//float TimeToCook = 12.0f;
	//float TimeToWarning = 16.0f;
	//float TimeToDanger = 19.0f;
	//float TimeToScorch = 22.0f;

	ChangeState(EPotState::HEATING, EPotState::BOILING, TimeToBoil);
	ChangeState(EPotState::BOILING, EPotState::COOKED, TimeToCook);
	ChangeState(EPotState::COOKED, EPotState::COOKED_WARNING, TimeToWarning);
	ChangeState(EPotState::COOKED_WARNING, EPotState::COOKED_DANGER, TimeToDanger);
	ChangeState(EPotState::COOKED_DANGER, EPotState::SCORCHING, TimeToScorch);
	ChangeState(EPotState::SCORCHING, EPotState::OVERCOOKED, TimeToOvercook);

}

void APot::ChangeState(EPotState CurState, EPotState NextState, float TransitionTime)
{
	if (CurState == PotState &&
		TransitionTime < TimeElapsed)
	{
		PotState = NextState;
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
		ChangeMaterialColor(FVector4(0.2f, 0.2f, 0.2f, 1.0f));
		break;
	}
	case EPotState::BOILING:
	{

		break;
	}
	case EPotState::COOKED:
	{
		bIsCooked = true;
		ChangeMaterialColor(FVector4(0.5f, 0.5f, 0.5f, 1.0f));

		FString TextureName = TEXT("CookingTick");
		SetTexture(TextureName);
		TimeEventComponent->AddEndEvent(0.5f, [this]()
			{
				TextureBillboard->SetVisibility(false);
			});
		break;
	}
	case EPotState::COOKED_WARNING:
	{
		ChangeMaterialColor(FVector4(0.1f, 0.1f, 0.1f, 1.0f));
		bCanBlink = true;

		FString TextureName = TEXT("BurnWarning");
		SetTexture(TextureName);
		BlinkTime = 1.0f;
		break;
	}
	case EPotState::COOKED_DANGER:
	{
		ChangeMaterialColor(FVector4(0.08f, 0.08f, 0.08f, 1.0f));
		BlinkTime = 0.5f;
		break;
	}
	case EPotState::SCORCHING:
	{
		ChangeMaterialColor(FVector4(0.04f, 0.04f, 0.04f, 1.0f));
		BlinkTime = 0.2f;
		break;
	}
	case EPotState::OVERCOOKED:
	{
		bIsCooked = false;

		ChangeMaterialColor(FVector4(0.02f, 0.02f, 0.02f, 1.0f));
		bCanBlink = false;
		TextureBillboard->bHiddenInGame = true;
		TextureBillboard->SetVisibility(false);
		break;
	}
	default:
		break;
	}
}

void APot::ChangeMaterialColor(FVector4 Color)
{
	int32 NumSoupMaterials = SoupSkeletalMeshComponent->GetMaterials().Num();
	for (int32 i = 1; i < NumSoupMaterials; i++)
	{
		SoupDynamicMaterial[i]->SetVectorParameterValue(FName("Tint"), Color);
	}
}

void APot::SetTexture(const FString& RowName)
{
	if (nullptr != TextureBillboard)
	{
		UTexture* Texture = UOC2GlobalData::GetResourceTexture(GetWorld(), *RowName);
		UTexture2D* NewTexture = Cast<UTexture2D>(Texture);
		TextureBillboard->SetSprite(NewTexture);

		TextureBillboard->SetVisibility(true);
		TextureBillboard->bHiddenInGame = false;
	}
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

void APot::ChangeSoupColor(float DeltaTime)
{
}

AIngredient* APot::GetRice()
{
	if (false == bIsCooked)
	{
		return nullptr;
	}

	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();
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

void APot::ResetPot()
{
	PrevPotState = EPotState::MAX;
	PotState = EPotState::IDLE;
	bIsRiceInPot = false;
	TimeElapsed = 0.0f;
	bCanBlink = false;
	bIsCombinationSuccessful = false;
	bIsCooked = false;

	ChangeNoneMaterial();
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
	FVector Offset = FVector(90, 0, -30);
	FRotator Rotation = FRotator(0, 90, 0);
	StaticMeshComponent->SetRelativeLocation(Offset);
	StaticMeshComponent->SetRelativeRotation(Rotation);

}

void APot::ForwardDetachToChef()
{
	FRotator Rotation = FRotator(0, 180, 0);
	StaticMeshComponent->SetRelativeLocation(InitPos);
	StaticMeshComponent->SetRelativeRotation(Rotation);
}
