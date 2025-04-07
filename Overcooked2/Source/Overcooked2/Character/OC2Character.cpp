// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"
#include "OC2CharacterTestTable.h"
#include "OC2CharacterTestChoppingTable.h"
#include "LevelContent/Cook/Pot.h"
#include "LevelContent/Table/ChoppingTable.h"
#include "LevelContent/Table/BurnerTable.h"
#include "LevelContent/Table/NonTable/GarbageCan.h"
#include "LevelContent/Table/NonTable/SinkTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Cooking/CaptureComponent2D.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AOC2Character::AOC2Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	//bUseControllerRotationYaw = false;

	GetMesh()->SetIsReplicated(true);


	GrabComponent = CreateDefaultSubobject<USceneComponent>("GrabPosition");
	GrabComponent->SetupAttachment(RootComponent);

	CaptureComponent = CreateDefaultSubobject<UCaptureComponent2D>("CaptureComponent");
	CaptureComponent->SetupAttachment(RootComponent);
	CaptureComponent->SetRelativeLocation({ 400.f,250.0f,-30.0f });
	CaptureComponent->SetRelativeRotation({ 0.0f, 210.0f, 0.0f });

	CheckOverlap = CreateDefaultSubobject<USphereComponent>("CheckOverlap");
	CheckOverlap->SetupAttachment(RootComponent);
	CheckOverlap->InitSphereRadius(100.0f);

	TimeEvent = CreateDefaultSubobject<UTimeEventComponent>("EventTimer");

	Plane = CreateDefaultSubobject<UStaticMeshComponent>("Plane");
	Plane->SetupAttachment(RootComponent);
}

void AOC2Character::MoveCharacter(const FInputActionValue& Value)
{
	FVector MovementInput = Value.Get<FVector>();

	MovementInput.Normalize();

	if (bIsChopping == true)
	{
		Chopping(false);
		Cast<AChoppingTable>(SelectedOC2Actor)->TimerSwitch(false);
	}

	if (bIsWashing == true)
	{
		Washing(false);
	}

	if (bIsDashing == false && bCanThrowing == false)
	{
		AddMovementInput(MovementInput);
	}
	//if (bIsDashing == false)
	//{
	//	GetMovementComponent()->AddInputVector(MovementInput);
	//}

	FQuat ActorRot = GetActorForwardVector().Rotation().Quaternion();
	FQuat TargetRot = FRotationMatrix::MakeFromX(MovementInput).Rotator().Quaternion();

	// 두 쿼터니언의 Yaw 차이를 계산
	float DeltaYaw = (TargetRot * ActorRot.Inverse()).Rotator().Yaw;

	// Alpha 값을 곱해 부드럽게 회전 적용
	AddControllerYawInput(DeltaYaw * Alpha);
}

// Called when the game starts or when spawned
void AOC2Character::BeginPlay()
{
	Super::BeginPlay();

	//CheckOverlap->OnComponentHit.AddDynamic(this, &AOC2Character::OnHit);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &AOC2Character::OnOverlapCheck);

	Plane->SetVisibility(IsLocallyControlled());

	// 임시 :
	//SetCharacterHead("Alien_Green");

}

void AOC2Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitMesh();
}

// Called every frame
void AOC2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, FString::Printf(TEXT("Bool: %s"), bCanThrowing ? TEXT("true") : TEXT("false")));

	CheckDash(DeltaTime);

	CheckInteract();

	DrawDebugSphere(GetWorld(), GrabComponent->GetComponentLocation(), TraceRadius, 20, FColor::Green, false, 0.0f);
}

void AOC2Character::CheckDash(float DeltaTime)
{
	if (bIsDashing == true)
	{
		DashTimer += DeltaTime;
		if (DashTimer > DashDuration)
		{
			bIsDashing = false;
		}
		else
		{

			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DashPower * DeltaTime, true);
		}
	}
}

// Called to bind functionality to input
void AOC2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOC2Character::MoveCharacter);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AOC2Character::Interact);
	EnhancedInputComponent->BindAction(CharacterAction, ETriggerEvent::Started, this, &AOC2Character::DoActionPress);
	EnhancedInputComponent->BindAction(CharacterAction, ETriggerEvent::Completed, this, &AOC2Character::DoActionRelease);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AOC2Character::Dash);

}

// 반드시 연결이 다 된 상태에서 호출할 것 BeginPlay() x
void AOC2Character::SetCharacterName_Implementation(const FString& Name)
{
	if (CharacterHeadMap.Contains(Name))
	{
		CharacterName = Name;
		OnRep_ChangeCharacter();
	}
}

void AOC2Character::OnRep_ChangeCharacter()
{
	ClearMaterials();
	FCharacterData Data = CharacterHeadMap[CharacterName];
	GetMesh()->SetMaterial(Data.MaterialIndex, Data.Material);
}

void AOC2Character::InitMesh()
{
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	ClearMaterials();
	for (int32 i = 1; i < Materials.Num(); i++)
	{
		FString Name = Materials[i]->GetName();
		if (Name.Contains("ChefHead"))
		{
			FString Key = Name.RightChop(15);
			CharacterHeadMap.Add(Key, FCharacterData(Materials[i], i));
			IndexToName.Add(Key);
		}
		if (Name.Contains("Knife"))
		{
			KnifeMaterial = { i, Materials[i] };
		}
		if (Name.Contains("Plate"))
		{
			PlateMaterial = { i, Materials[i] };
		}
	}

	//GetMesh()->SetMaterial(CharacterHeadMap["Alien_Green"].MaterialIndex, CharacterHeadMap["Alien_Green"].Material);
}

void AOC2Character::ClearMaterials()
{
	//0 is ChefBody
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	for (int32 i = 1; i < Materials.Num(); i++)
	{
		FString Name = Materials[i]->GetName();
		// hat index
		if (i == 36) continue;
		GetMesh()->SetMaterial(i, TransparentMat);
	}
}

//상호작용 : Space Key
void AOC2Character::Interact_Implementation()
{
	// 만약 지금 상호작용을 시도할 수 있는 개체가 있으면
	if (!SelectedOC2Actor)
	{
		Drop();
		return;
	}

	if (bIsChopping) Chopping(false);
	ACooking* Cooking = Cast<ACooking>(SelectedOC2Actor);
	ACookingTable* Table = Cast<ACookingTable>(SelectedOC2Actor);

	if (Cooking != nullptr)
	{
		// 잡고 있는게 없으면 Cooking은 무조건 잡을 수 있는 액터
		if (GrabbedObject == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Hold"));
			Grab(Cast<ACooking>(SelectedOC2Actor));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, SelectedOC2Actor->GetName());
		}
		// 잡고 있는게 있으면
		else
		{
			if (AIngredient* Ingredient = Cast<AIngredient>(Cooking))
			{
				UE_LOG(LogTemp, Log, TEXT("This is an ingredient!"));
				if (GrabbedObject != nullptr)
				{
					Drop();
				}
			}
			// 상호작용을 접시랑 할때
			else if (APlate* Plate = Cast<APlate>(Cooking))
			{
				AIngredient* GrabbedIng = Cast<AIngredient>(GrabbedObject);
				APot* GrabbedPot = Cast<APot>(GrabbedObject);
				UE_LOG(LogTemp, Log, TEXT("This is a plate!"));
				if (GrabbedIng != nullptr)
				{
					Plate->Add(GrabbedIng);
					//성공하면 GrabbedObject를 null로 만든다.
					if (Plate->IsCombinationSuccessful())
					{
						GrabbedObject = nullptr;
					}
				}
				else if (GrabbedPot != nullptr)
				{
					AIngredient* Rice = GrabbedPot->GetRice();
					Plate->Add(Rice);
					//성공하면 GrabbedObject를 null로 만든다.
				}
			}
			else if (APot* Pot = Cast<APot>(Cooking))
			{
				UE_LOG(LogTemp, Log, TEXT("This is a pot!"));
				Pot->Add(Cast<AIngredient>(GrabbedObject));
				if (Pot->IsCombinationSuccessful())
				{
					GrabbedObject = nullptr;
				}
			}
			else
			{
				// 기본적으로 그냥 내려놓기
				Drop();
			}
		}
	}
	else if (Table != nullptr)
	{
		if (Table->IsA(AGarbageCan::StaticClass()) == true)
		{
			APlate* Plate = Cast<APlate>(GrabbedObject);
			AIngredient* Ingredient = Cast<AIngredient>(GrabbedObject);
			if (Plate != nullptr)
			{
				Plate->SetPlateState(EPlateState::EMPTY);
			}
			else if (Ingredient != nullptr)
			{
				Table->PlaceItem(Ingredient);
			}
		}
		else
		{
			// 테이블과 상호작용을 시도한다.
			ACooking* Cook = Table->Interact(this);
			// 잡고있는 물건이 없고, 테이블에 올려진 물체가 있는 경우
			if (GrabbedObject == nullptr && Cook != nullptr)
			{
				Grab(Cook);
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, Cook->GetName());
			}
			// 잡은 물건이 있는데 테이블이 비어있으면
			else if (GrabbedObject != nullptr && Cook == nullptr)
			{
				if (Table->IsA<ABurnerTable>() == true)
				{
					if (GrabbedObject->IsA<APot>() == true)
					{
						Table->PlaceItem(GrabbedObject);
						GrabbedObject = nullptr;
					}
					else
					{
						return;
					}
				}
				else
				{
					// 다른 액터에 Attach하게 되면 수동으로 Detach할 필요 X
					Table->PlaceItem(GrabbedObject);
					GrabbedObject = nullptr;
				}
			}
			//내가 뭔가 잡고 있는데 Cook을 리턴받았으면 Cook을 다시 돌려놔야 한다.
			else if (GrabbedObject != nullptr && Cook != nullptr)
			{
				APot* Pot = Cast<APot>(Cook);
				APlate* Plate = Cast<APlate>(Cook);

				//잡고있는 타입 캐스트
				AIngredient* GrabIng = Cast<AIngredient>(GrabbedObject);
				APlate* GrabPlate = Cast<APlate>(GrabbedObject);
				APot* GrabPot = Cast<APot>(GrabbedObject);
				if (GrabIng != nullptr)
				{
					if (Pot != nullptr)
					{
						Pot->Add(GrabIng);
						if (Pot->IsCombinationSuccessful())
						{
							GrabbedObject = nullptr;
						}
					}
					else if (Plate != nullptr)
					{
						Plate->Add(GrabIng);
						if (Plate->IsCombinationSuccessful())
						{
							GrabbedObject = nullptr;
						}
					}
				}
				else if (GrabPlate != nullptr)
				{

				}
				else if (GrabPot != nullptr)
				{
					if (Plate != nullptr)
					{
						// GrabPot에서 Pot은 초기화
						Plate->Add(GrabPot->GetRice());
					}
				}

				Table->PlaceItem(Cook);
			}
		}
	}
}

void AOC2Character::Grab_Implementation(ACooking* Cook)
{
	GrabbedObject = Cook;
	GrabbedObject->AttachToChef(this);

	auto DataTable = GrabbedObject->GetIngredientDataTable();
	if (DataTable)
	{
		GrabbedObject->SetActorRelativeRotation(DataTable->Rotation);
		GrabbedObject->SetActorLocation(GrabComponent->GetComponentTransform().TransformPosition(DataTable->Location));
	}
}

void AOC2Character::Drop_Implementation()
{
	// 내가 들고 있는 물건이 있을때
	if (GrabbedObject != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Drop"));
		// 들고 있는 물체에 대해 상호작용을 실행한다. 바닥에 내려놓는다는 뜻.
		GrabbedObject->DetachFromChef(this);

		auto DataTable = GrabbedObject->GetIngredientDataTable();
		if (DataTable)
		{
			GrabbedObject->SetActorRotation(DataTable->Rotation);
		}

		GrabbedObject->SetActorLocation(GrabComponent->GetComponentLocation());
		GrabbedObject = nullptr;
	}
}

void AOC2Character::DoActionPress_Implementation()
{
	if (GrabbedObject == nullptr && SelectedOC2Actor == nullptr)
	{
		return;
	}
	if (GrabbedObject != nullptr)
	{
		if (!GrabbedObject->IsA(APlate::StaticClass()))
		{
			bCanThrowing = true;
		}
	}
	else
	{
		ACookingTable* Table = Cast<ACookingTable>(SelectedOC2Actor);
		if (Table != nullptr)
		{
			if (Table->IsA<AChoppingTable>())
			{
				Cast<AChoppingTable>(Table)->ChopIngredient(this);
			}
			if (Table->IsA<ASinkTable>())
			{
				Cast<ASinkTable>(Table)->DoTheDishes(this);
				//Washing(true);
			}
		}
	}
}

void AOC2Character::DoActionRelease_Implementation()
{
	if (bCanThrowing == true)
	{
		Throwing();
	}
}



void AOC2Character::Throwing_Implementation()
{
	AIngredient* ThrowingObject = Cast<AIngredient>(GrabbedObject);
	if (GrabbedObject && ThrowingObject)
	{
		// 1️⃣ 액터의 루트 컴포넌트를 가져오기
		UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(GrabbedObject->GetRootComponent());

		if (PrimitiveComp)
		{
			//GrabbedObject->SetActorLocation(GrabComponent->GetComponentTransform().TransformPosition(FVector(0, 0, 100)));
			
			GrabbedObject->DetachFromChef(this);
			GrabbedObject->SetActorLocation(GrabComponent->GetComponentTransform().TransformPosition(FVector(30, 0, 80)));
			
			ThrowingObject->SetThrower(this);
			ThrowingObject->SetThrowing(true);

			// 4️⃣ 던질 방향과 세기 설정
			FVector ThrowDirection = (GetActorForwardVector() + FVector(0, 0, 0.2f)).GetSafeNormal();  // 캐릭터가 바라보는 방향

			float ThrowStrength = 1200.0f;  // 던지는 힘 조절

			// 5️⃣ 물리적 임펄스 추가 (던지기)
			PrimitiveComp->AddImpulse(ThrowDirection * ThrowStrength, NAME_None, true);
		}

		// 6️⃣ 잡고 있던 객체 초기화
		bCanThrowing = false;
		GrabbedObject = nullptr;
	}
}

void AOC2Character::Chopping_Implementation(bool State)
{
	bIsChopping = State;
	OnRep_KnifeSet();
	if (State == false)
	{
		Cast<AChoppingTable>(SelectedOC2Actor)->TimerSwitch(false);
	}
}

void AOC2Character::CheckInteract()
{
	if (GrabComponent == nullptr)
	{
		return; // SceneComponent가 유효한지 확인(사실 무조건 유효해야함)
	}

	FVector TraceLocation = GrabComponent->GetComponentLocation(); // SceneComponent 위치 가져오기

	// 감지할 오브젝트 유형 설정 (예: WorldDynamic)
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	// 트레이스 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신 무시

	// 결과 저장 배열
	TArray<FHitResult> HitResults;

	// Sphere Trace 실행
	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		TraceLocation, TraceLocation,  // Start와 End 동일 (정지된 구)
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(TraceRadius),
		QueryParams
	);

	//UE_LOG(LogTemp, Log, TEXT("%d"), HitResults.Num());

	if (bHit)
	{
		//DrawDebugSphere(GetWorld(), GetActorLocation(), 200.0f, 20, FColor::Red, true);

		HitResults.Sort([TraceLocation](const FHitResult& A, const FHitResult& B) {
			return FVector::DistSquared(TraceLocation, A.GetActor()->GetActorLocation()) < FVector::DistSquared(TraceLocation, B.GetActor()->GetActorLocation());
			});

		// maybe Interactable.
		AOC2Actor* ClosestActor = Cast<AOC2Actor>(HitResults[0].GetActor());
		if (SelectedOC2Actor != nullptr)
		{
			if (ClosestActor != SelectedOC2Actor)
			{
				SelectedOC2Actor->SetHighlight(false);
			}
			//UMaterialInstanceDynamic* DynamicMat = Mesh->CreateDynamicMaterialInstance(0);
			//if (DynamicMat)
			//{
			//	DynamicMat->SetScalarParameterValue(TEXT("Brightness"), 2.0f);
			//}
		}
		SelectedOC2Actor = ClosestActor;
		if (!SelectedOC2Actor->IsHighlighted())
		{
			SelectedOC2Actor->SetHighlight(true);
		}
		//SelectedOC2Actor->Highlight();
	}
	else
	{
		if (SelectedOC2Actor != nullptr)
		{
			//Cast<AOC2CharacterTestTable>(SelectedOC2Actor)->OffHighlight();
			SelectedOC2Actor->SetHighlight(false);
		}
		SelectedOC2Actor = nullptr;
	}
}

void AOC2Character::Dash_Implementation()
{
	if (bIsDashing == false)
	{
		bIsDashing = true;
		DashTimer = 0.0f;
	}
	//FVector DashDirection = GetActorForwardVector(); // 바라보는 방향
	//LaunchCharacter(DashDirection * DashPower, true, false); // 대시 실행

}

void AOC2Character::Washing_Implementation(bool State)
{
	bIsWashing = State;
	OnRep_PlateSet();
}

void AOC2Character::OnRep_KnifeSet()
{
	if (bIsChopping)
	{
		GetMesh()->SetMaterial(KnifeMaterial.Key, KnifeMaterial.Value);
	}
	else
	{
		GetMesh()->SetMaterial(KnifeMaterial.Key, TransparentMat);
	}
}

void AOC2Character::OnRep_PlateSet()
{
	if (bIsWashing)
	{
		GetMesh()->SetMaterial(PlateMaterial.Key, PlateMaterial.Value);
	}
	else
	{
		GetMesh()->SetMaterial(PlateMaterial.Key, TransparentMat);
	}
}


void AOC2Character::StopDash()
{
	bIsDashing = false;
	GetCharacterMovement()->StopMovementImmediately();
}

void AOC2Character::OnOverlapCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AIngredient* Ingredient = Cast<AIngredient>(OtherActor);
		if (Ingredient != nullptr) // 캡슐과 충돌한 경우w
		{
			if (Ingredient->IsThrowing() && Ingredient->GetThrower() != this && GrabbedObject == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Hit");
				Ingredient->AttachToChef(this);
				auto DataTable = Ingredient->GetIngredientDataTable();
				if (DataTable)
				{
					Ingredient->SetActorRelativeRotation(DataTable->Rotation);
					Ingredient->SetActorLocation(GrabComponent->GetComponentTransform().TransformPosition(DataTable->Location));
				}
				Ingredient->SetThrowing(false);
				GrabbedObject = Ingredient;
				FVector Dir = (Ingredient->GetThrower()->GetActorLocation() - GetActorLocation());
				Dir.Z = 0;
				Dir = Dir.GetSafeNormal();

				if (GetController() != nullptr)
				{
					GetController()->SetControlRotation(Dir.Rotation());
				}

				DrawDebugSphere(GetWorld(), OverlappedComponent->GetComponentLocation(), CheckOverlap->GetScaledSphereRadius(), 20, FColor::Green, false, 0.0f);

			}
		}
	}
}



void AOC2Character::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Hit");
		AIngredient* Ingredient = Cast<AIngredient>(OtherActor);
		if (Ingredient) // 캡슐과 충돌한 경우
		{

		}
	}
}

void AOC2Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOC2Character, GrabbedObject);
	DOREPLIFETIME(AOC2Character, bIsDashing);
	DOREPLIFETIME(AOC2Character, DashTimer);
	DOREPLIFETIME(AOC2Character, DashTimer);
	DOREPLIFETIME(AOC2Character, bCanThrowing);
	DOREPLIFETIME(AOC2Character, bIsChopping);
	DOREPLIFETIME(AOC2Character, bIsWashing);
	DOREPLIFETIME(AOC2Character, CharacterName);
}




