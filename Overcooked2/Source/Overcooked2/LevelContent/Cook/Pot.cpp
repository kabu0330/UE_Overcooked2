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
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>
#include <LevelContent/Cook/Widget/CookStatusWidget.h>
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/IngredientIconWidget.h>

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SetReplicateMovement(true);

	CookingType = ECookingType::ECT_POT;

	SoupSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SoupSkeletalMeshComponent"));
	SoupSkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	SoupSkeletalMeshComponent->SetIsReplicated(true);

	GaugeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	GaugeWidgetComponent->SetupAttachment(RootComponent);

	StatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
	StatusWidgetComponent->SetupAttachment(RootComponent);

	IconWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidgetComponent"));
	IconWidgetComponent->SetupAttachment(RootComponent);

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent"));

	StaticMeshComponent->SetRelativeLocation(InitPos);
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();

	NoneMaterial = LoadNoneMaterial(); // 여기서 해줘야 클라도 NULL 머티리얼 생성된다.
	SetSoupMaterial();
	ChangeNoneMaterial();

	InitGaugeWidget();
	InitStatusWidget();
	InitIconWidget();
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

void APot::InitGaugeWidget()
{
	// 위젯 클래스 지정
	GaugeWidgetComponent->SetWidgetClass(SubclassGaugeWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = GaugeWidgetComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		GaugeWidget = Cast<UGaugeTextureWidget>(UserWidget);
		GaugeWidget->SetPosition(FVector2D(0.0f, 50.0f));
	}
	InitWidgetSetting(GaugeWidgetComponent);
}

void APot::InitStatusWidget()
{
	StatusWidgetComponent->SetWidgetClass(SubclassStatusWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = StatusWidgetComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		StatusWidget = Cast<UCookStatusWidget>(UserWidget);
	}
	InitWidgetSetting(StatusWidgetComponent);
}

void APot::InitIconWidget()
{
	IconWidgetComponent->SetWidgetClass(SubclassIconWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = IconWidgetComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		IconWidget = Cast<UIngredientIconWidget>(UserWidget);
		SetIcon(TEXT("MissingIngredient"));
	}
	InitWidgetSetting(IconWidgetComponent);
	IconWidgetComponent->bHiddenInGame = false;

}

void APot::InitWidgetSetting(UWidgetComponent* WidgetComponent)
{
	WidgetComponent->SetDrawAtDesiredSize(true);   // 위젯의 실제 크기로 렌더
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f)); // 중심 정렬
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 월드 공간에서 3D처럼 보이게
	WidgetComponent->bHiddenInGame = true;

	// 카메라를 향하도록 설정
	WidgetComponent->SetTwoSided(true);
	WidgetComponent->SetTickWhenOffscreen(true);
}

void APot::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

	Cook(DeltaTime);
	SetAction();
	BlinkTexture(DeltaTime);
	UpdateTextureVisibilityOnTable();
	ChangeSoupColor(DeltaTime);

	UpdateGaugeWidget();

}

void APot::UpdateGaugeWidget()
{
	if (0.01f >= CookingTimeRatio)
	{
		return;
	}
	if (1.0f <= CookingTimeRatio)
	{
		GaugeWidgetComponent->bHiddenInGame = true; // 100%면 숨기고
		return;
	}
	else if (nullptr != GaugeWidget) // 100%도 아니고 위젯도 정상적으로 세팅되어 있으면
	{
		GaugeWidgetComponent->bHiddenInGame = false;
		GaugeWidget->SetProgressTimeRatio(CookingTimeRatio); // 비율대로 채워주고
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

	bIsRiceInPot = true; // bool 값으로 재료가 들어왔는지 체크할 것임
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
	if (nullptr == CookingTable)
	{
		return false;
	}
	ABurnerTable* BurnerTable = Cast<ABurnerTable>(CookingTable);
	if (nullptr == BurnerTable) // 2. 버너 위에 있냐
	{
		return false;
	}
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

	CookingTimeRatio = TimeElapsed / TimeToCook;

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
		// 초기 머티리얼 색상 설정
		ChangeMaterialColor(FVector4(0.2f, 0.2f, 0.2f, 1.0f));
		CurrentColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// 0.2에서 0.6까지 컬러 러프
		bColorChanging = true;
		TargetColor = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);


		SetIcon(TEXT("RiceIcon"));
		break;
	}
	case EPotState::BOILING:
	{
		break;
	}
	case EPotState::COOKED:
	{
		bIsCooked = true; // 조리완료

		bColorChanging = true;
		TargetColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f);

		FString TextureName = TEXT("CookingTick");
		UTexture2D* Texture = GetTexture(TextureName);
		StatusWidget->SetStatusTexture(Texture);
		StatusWidgetComponent->bHiddenInGame = false;

		TimeEventComponent->AddEndEvent(0.5f, [this]()
			{
				StatusWidgetComponent->bHiddenInGame = true;
			});
		break;
	}
	case EPotState::COOKED_WARNING:
	{
		bColorChanging = true;
		TargetColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// 경고 텍스처 활성화
		bIsBlinking = true;

		FString TextureName = TEXT("BurnWarning");
		UTexture2D* Texture = GetTexture(TextureName);
		StatusWidget->SetStatusTexture(Texture);
		StatusWidgetComponent->bHiddenInGame = false;
		CanBlink();

		BlinkTime = 0.5f;
		break;
	}
	case EPotState::COOKED_DANGER:
	{
		bColorChanging = true;
		TargetColor = FLinearColor(0.07f, 0.07f, 0.07f, 1.0f);

		BlinkTime = 0.2f;
		break;
	}
	case EPotState::SCORCHING:
	{
		bColorChanging = true;
		TargetColor = FLinearColor(0.03f, 0.03f, 0.03f, 1.0f);

		BlinkTime = 0.1f;
		break;
	}
	case EPotState::OVERCOOKED:
	{
		bIsCooked = false; // 조리실패

		bColorChanging = true;
		TargetColor = FLinearColor(0.01f, 0.01f, 0.01f, 1.0f);

		bIsBlinking = false;

		StatusWidgetComponent->bHiddenInGame = true;
		SetIcon(TEXT("BurntFood"));
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

UTexture2D* APot::GetTexture(const FString& RowName)
{
	UTexture* NewTexture = UOC2GlobalData::GetResourceTexture(GetWorld(), *RowName);
	UTexture2D* NewTexture2D = Cast<UTexture2D>(NewTexture);
	return NewTexture2D;
}

void APot::SetIcon(const FString& RowName)
{
	FString TextureName = RowName;
	UTexture2D* Texture = GetTexture(TextureName);
	IconWidget->SetTexture(Texture);
}

void APot::CanBlink()
{
	// 버너 테이블일 때만
	if (nullptr != Cast<ABurnerTable>(CookingTable))
	{
		bCanBlink = true; // 블링크가 가능하다.
		TimeEventComponent->AddEndEvent(0.09f, [this]()
			{
				bCanBlink = false; // 블링크 업데이트가 정상적으로 되도록 잠깐 값을 바꿔줬다가 원복
			});
	}
}

void APot::BlinkTexture(float DeltaTime)
{
	if (false == bIsBlinking)
	{
		return;
	}
	
	// 텍스처가 보이는 중이면
	if (false == StatusWidgetComponent->bHiddenInGame)
	{
		BlinkTimeElapsed += DeltaTime;
	}

	// 텍스처가 일정 시간 이미 보였다면
	if (BlinkTime <= BlinkTimeElapsed && false == StatusWidgetComponent->bHiddenInGame)
	{
		StatusWidgetComponent->bHiddenInGame = true; // 다시 텍스처 끄고
		BlinkTimeElapsed = 0.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("텍스처 끔"));

		TimeEventComponent->AddEndEvent(BlinkTime, [this]() 
			{
				StatusWidgetComponent->bHiddenInGame = false; // 일정 시간 뒤에 다시 켠다.
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("텍스처 켬"));
			});
	}
}

void APot::UpdateTextureVisibilityOnTable()
{
	if (nullptr == CookingTable) // 테이블에 있지 않다면
	{
		StatusWidgetComponent->bHiddenInGame = true; // 숨김처리
		return;
	}

	// 버너 테이블에 다시 올라왔는데, 블링크 효과가 진행중인 상태에서 숨김처리가 되어있다면,
	else if (nullptr != Cast<ABurnerTable>(CookingTable) && 
		true == bCanBlink && 
		true == bIsBlinking && 
		true == StatusWidgetComponent->bHiddenInGame)
	{	
		StatusWidgetComponent->bHiddenInGame = false; // 블링크가 되도록 설정
	}
}

void APot::ChangeSoupColor(float DeltaTime)
{
	if (true == bColorChanging)
	{
		CurrentColor = FMath::Lerp(CurrentColor, TargetColor, DeltaTime * ColorChangeSpeed);

		// 실제 머티리얼에 적용
		FVector4 Color = FVector4(CurrentColor.R, CurrentColor.G, CurrentColor.B, CurrentColor.A);
		ChangeMaterialColor(Color);

		// 목표에 거의 도달했으면 종료
		if (FVector(CurrentColor - TargetColor).Size() < 0.001f)
		{
			CurrentColor = TargetColor;
			bColorChanging = false;
		}
	}
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
	bIsCombinationSuccessful = false;
	bIsCooked = false;
	bIsBlinking = false;
	CookingTimeRatio = 0.0f;
	
	StatusWidgetComponent->bHiddenInGame = true;
	GaugeWidgetComponent->bHiddenInGame = true;

	SetIcon(TEXT("MissingIngredient"));
	IconWidgetComponent->bHiddenInGame = false;

	ChangeNoneMaterial();
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
	DOREPLIFETIME(APot, bIsBlinking);
	DOREPLIFETIME(APot, bCanBlink);
	DOREPLIFETIME(APot, bIsCombinationSuccessful);
	DOREPLIFETIME(APot, TargetColor);
}

// Table이 SetCookingTable 함수를 호출해주면 여기서 세팅
void APot::ForwardCookingTable(ACookingTable* Table)
{
	CookingTable = Table;

	CanBlink(); // 블링크 효과 활성화 여부 체크 후 켬
}

// 캐릭터가 냄비를 들어올렸을 때, 호출
void APot::ForwardAttachToChef()
{
	CookingTable = nullptr;
	FVector Offset = FVector(90, 0, -30);
	FRotator Rotation = FRotator(0, 90, 0);
	StaticMeshComponent->SetRelativeLocation(Offset);
	StaticMeshComponent->SetRelativeRotation(Rotation);

	bCanBlink = false; // 블링크 효과 끔

}

void APot::ForwardDetachToChef()
{
	FRotator Rotation = FRotator(0, 180, 0);
	StaticMeshComponent->SetRelativeLocation(InitPos);
	StaticMeshComponent->SetRelativeRotation(Rotation);
}
