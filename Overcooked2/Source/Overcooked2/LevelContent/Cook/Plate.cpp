// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Plate.h"
#include <LevelContent/Cook/Ingredient.h>
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include <Global/Data/OC2GlobalData.h>
#include "Components/BillboardComponent.h"  

// Sets default values
APlate::APlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CookingType = ECookingType::ECT_PLATE;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientMesh"));
	IngredientMesh->SetIsReplicated(true); // 컴포넌트 네트워크 동기화

	FVector Scale = FVector(2.0f, 2.0f, 2.0f);
	StaticMeshComponent->SetRelativeScale3D(Scale);


	int MaxTexture = 3;
	for (int i = 0; i < MaxTexture; i++)
	{
		FString ComponentName = FString::Printf(TEXT("TextureBillboard_%d"), i);
		UBillboardComponent* BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(*ComponentName);
		BillboardComponent->SetupAttachment(RootComponent);
		TextureBillboards.Add(BillboardComponent);
	}

	// Debug
	SetActorLocation(FVector(0.0f, -200.0f, 10.0f));
}

void APlate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlate, Ingredients);
	DOREPLIFETIME(APlate, IngredientMesh);
	DOREPLIFETIME(APlate, PlateState);
}

// Called when the game starts or when spawned
void APlate::BeginPlay()
{
	ACooking::BeginPlay();
	
}

// Called every frame
void APlate::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);

}

bool APlate::IsDirtyPlate()
{
	if (nullptr == StaticMeshComponent)
	{
		return false;
	}

	return PlateState == EPlateState::DIRTY;
}

void APlate::WashPlate_Implementation()
{
	if (true == IsDirtyPlate())
	{
		PlateState = EPlateState::EMPTY;
		SetMesh();
	}
}

void APlate::SetPlateState_Implementation(EPlateState State)
{
	PlateState = State;
	SetMesh();
}

void APlate::SetMesh()
{
	UTexture* Texture = nullptr;
	if (EPlateState::DIRTY == PlateState)
	{
		Texture = DirtyTexture;
	}
	else
	{
		Texture = CleanTexture;
	}
	SetMaterialTexture(Texture);
}

void APlate::SetMaterialTexture(UTexture* Texture)
{
	// 1. 스태틱 메시의 머티리얼을 바꿀건데

	// 2. 이미 동적으로 생성한 머티리얼 인스턴스 다이나믹이 존재하면 
	UMaterialInstanceDynamic* MaterialInstanceDynamic = Cast<UMaterialInstanceDynamic>(StaticMeshComponent->GetMaterial(0));
	if (nullptr != MaterialInstanceDynamic)
	{
		// 3. 기존 머티리얼 인스턴스 다이나믹을 그대로 사용하고
		MaterialInstanceDynamic->SetTextureParameterValue(FName(TEXT("DiffuseAdd")), Texture);
		StaticMeshComponent->SetMaterial(0, MaterialInstanceDynamic);
		return;
	}

	// 4. 기존에 만들어진 머티리얼 인스턴스 다이나믹이 없다면 == SetMaterial이 처음이라면
	UMaterialInterface* Material = StaticMeshComponent->GetMaterial(0);
	if (nullptr != Material)
	{
		// 5. 이제 기존의 머티리얼은 안쓰고 머티리얼 인스턴스 다이나믹을 만들어서 쓸 것이다.
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (nullptr != DynamicMaterial)
		{
			// 6. 바꿀 텍스처를 에디터에서 설정해줬다면 그걸로 바꿔라.
			if (nullptr != Texture)
			{
				DynamicMaterial->SetTextureParameterValue(FName(TEXT("DiffuseAdd")), Texture);
				StaticMeshComponent->SetMaterial(0, DynamicMaterial);
			}
		}
	}
}

bool APlate::CanPlaceOnPlate(AIngredient* Ingredient)
{
	if (ECookingType::ECT_INGREDIENT != Ingredient->GetCookingType())
	{
		return false;
	}
	if (EPlateState::COMPLETED == PlateState || EPlateState::DIRTY == PlateState)
	{	// 이미 완성된 요리나 세척 전의 접시는 재료를 올릴 수 없다.
		return false;
	}
	if (EIngredientState::EIS_NONE == Ingredient->GetCurIngredientState())
	{	// 손질되지 않은 재료는 접시에 올릴 수 없다.
		return false;
	}
	if (nullptr == StaticMeshComponent)
	{
		return false;
	}
	return true;
}

void APlate::Add_Implementation(AIngredient* Ingredient)
{
	bIsCombinationSuccessful = false;

	if (false == CanPlaceOnPlate(Ingredient))
	{
		return;
	}

	// 1. 손질된 재료를 추가한다.
	FRecipe Recipe;
	Recipe.IngredientType = Ingredient->GetIngredientType();
	Recipe.IngredientState = Ingredient->GetCurIngredientState();
	Ingredients.Add(Recipe);

	// 2. RecipeDataTable과 비교하여 데이터 테이블에 해당 재료조합이 존재하는지 확인
	FPlateInitData InitData = UOC2GlobalData::GetPlateMesh(GetWorld(), Ingredients);

	// 3-1. 데이터를 획득하는데 실패했다면
	if (nullptr == InitData.StaticMesh) 
	{
		Ingredients.Pop(); // 재료 자료구조에서 제거하고 리턴
		return;
	}
	else // 3-2. 데이터를 획득하는데 성공했다면 
	{
		IngredientMesh->SetStaticMesh(InitData.StaticMesh); // 메시 변경
		if (nullptr != IngredientMesh)
		{
			// 3-3. 접시 위에 올라갈 요리 메시 세팅
			SetIngredinetMesh(InitData);

			// 3-5. 기존에 존재하는 재료는 월드에서 삭제
			Ingredient->RequestOC2ActorDestroy();

			// 4. Texture 추가
			SetIngredinetTextures(InitData);
			return;
		}
	}
	return;
}

void APlate::SetIngredinetMesh(FPlateInitData Data)
{
	// 1. 물리 잠시 끄고
	SetSimulatePhysics(false); // 컴포넌트와 충돌로 날아가는 움직이는 것을 방지하기 위해 물리를 잠시 끈다.

	// 2. IngredientMesh의 충돌체와 물리를 끈다.
	IngredientMesh->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IngredientMesh->SetCollisionProfileName(TEXT("NoCollision"));
	IngredientMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IngredientMesh->SetSimulatePhysics(false);

	// 3. Offset
	IngredientMesh->SetRelativeLocation(Data.OffsetLocation);
	IngredientMesh->SetRelativeRotation(Data.OffsetRotation);
	IngredientMesh->SetRelativeScale3D(Data.OffsetScale);

	// 4. 물리 다시 켜고
	SetSimulatePhysics(true);
	bIsCombinationSuccessful = true;
}

void APlate::SetIngredinetTextures(FPlateInitData Data)
{
	TArray<UTexture*> Textures = Data.IngredientTextures;
	if (true == Textures.IsEmpty())
	{
		return;
	}

	int32 NumTextures = Textures.Num(); // 텍스처 개수
	int32 NumBillboard = TextureBillboards.Num(); // 텍스처 최대 개수 : 3개(생성자에서 설정함)
	int32 NumRows = (NumTextures + 1) / 2; // 몇 행인지. (올림 처리)

	float BaseZ = 50.0f; // 최초 z축 위치
	float VertialSpacing = 40.0f; // 상하 간격
	float HorizontalSpacing = 40.0f; // 좌우 간격
	for (int32 i = 0; i < NumTextures; i++) // 이번에 들어온 텍스처 수만큼 반복
	{
		int32 RowIndex = i / 2; 
		int32 ColIndex = i % 2;

		int32 ActualRow = NumRows - 1 - RowIndex; // 실제 행 0번부터
		float CurrentZ = BaseZ + ActualRow * VertialSpacing; // 행 간 간격

		bool bIsRowFull = true;
		// 마지막 텍스처인데 홀수라면 그 텍스처만 가운데에 맞춰야 하니까
		if ((RowIndex == NumRows - 1) && (NumTextures % 2 != 0)) 
		{
			bIsRowFull = false; // 해당 변수를 이용해서 
		}

		float CurrentY = 0.0f;
		if (true == bIsRowFull) // 한 행에 두 개씩 존재하면 좌우 간격 맞추고
		{
			if (0 == ColIndex)
			{
				CurrentY = -HorizontalSpacing / 2.0f;
			}
			else
			{
				CurrentY = HorizontalSpacing / 2.0f;
			}
		}
		else // 한 행에 남은 텍스처가 하나면 가운데 맞추자.
		{
			CurrentY = 0.0f;
		}

		FVector NewLocation(0.0f, CurrentY, CurrentZ);

		UTexture2D* Texture = Cast<UTexture2D>(Textures[i]);
		if (nullptr != Texture)
		{
			TextureBillboards[i]->SetSprite(Texture);
			TextureBillboards[i]->bHiddenInGame = false; // 안 켜면 안보임
			TextureBillboards[i]->SetRelativeLocation(NewLocation);
		}
	}
}
