// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"
#include "OC2CharacterTestTable.h"
#include "OC2CharacterTestChoppingTable.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	}

	if (bIsDashing == false && bCanThrowing == false)
	{
		AddMovementInput(MovementInput);
	}
	//if (bIsDashing == false)
	//{
	//	GetMovementComponent()->AddInputVector(MovementInput);
	//}


	float CurrentYaw = GetActorRotation().Yaw;
	float TargetYaw = MovementInput.Rotation().Yaw;

	float InterpSpeed = 10.0f; // 회전 속도 조절 가능
	float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

	float YawDelta = FMath::FindDeltaAngleDegrees(CurrentYaw, NewYaw);

	AddControllerYawInput(YawDelta);


}

// Called when the game starts or when spawned
void AOC2Character::BeginPlay()
{
	Super::BeginPlay();

	InitMesh();

	//CheckOverlap->OnComponentHit.AddDynamic(this, &AOC2Character::OnHit);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &AOC2Character::OnOverlapCheck);
	// 임시 :
	//SetCharacterHead("Alien_Green");

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

void AOC2Character::CheckDash_Implementation(float DeltaTime)
{
	if (bIsDashing)
	{
		DashTimer += DeltaTime;
		if (DashTimer > DashDuration)
		{
			bIsDashing = false;
		}
		else
		{
			LaunchCharacter(GetActorForwardVector() * DashPower, true, false);
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
		CharacterName = Name;
	OnRep_ChangeCharacter();
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
			Knife = { i, Materials[i] };
		}
	}

	GetMesh()->SetMaterial(CharacterHeadMap["Alien_Green"].MaterialIndex, CharacterHeadMap["Alien_Green"].Material);
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
	/*SetCharacterName(IndexToName[FMath::RandRange(0, CharacterHeadMap.Num() - 1)]);
	OnRep_ChangeCharacter();*/

	// 만약 지금 상호작용을 시도할 수 있는 개체가 있으면
	if (SelectedOC2Actor != nullptr)
	{
		if (bIsChopping)
			Chopping(false);
		////만약 이 액터가 재료나 요리 접시인 경우
		//if (SelectedOC2Actor->IsA(ACooking::StaticClass()))
		//{
		//	if (GrabbedObject == nullptr)
		//	{
		//		UE_LOG(LogTemp, Log, TEXT("Hold"));
		//		// 그냥 든다.
		//		Grab(Cast<ACooking>(SelectedOC2Actor));
		//	}
		//	else
		//	{
		//		Drop();
		//	}
		//}

		// 컴포넌트로 뺄지 안뺄지 모르겠음.
		// 일단 Plate인 경우
		if (auto Plate = Cast<APlate>(SelectedOC2Actor))
		{
			// 내가 들고 있는게 재료라면
			if (auto Ingredient = Cast<AIngredient>(GrabbedObject))
			{
				//Plate에 Ingredient추가를 시도한다.
				Plate->Add(Ingredient);
				//성공하면 GrabbedObject를 null로 만든다.
				if (Plate->IsCombinationSuccessful())
				{
					GrabbedObject = nullptr;
				}
			}
			// 내가 들고 있는게 없으면 잡는다.
			else if (GrabbedObject == nullptr)
			{
				Grab(Plate);
			}
		}
		//상호작용 할 녀석이 재료 라면 
		else if (auto Ingredient = Cast<AIngredient>(SelectedOC2Actor))
		{
			// 잡고 있는게 있으면 그냥 떨어뜨리고
			if (GrabbedObject)
			{
				Drop();
			}
			// 없으면 그 재료를 집는다.
			else
			{
				Grab(Ingredient);
			}
		}
		//만약 이 액터가 테이블인 경우 ???(여기가 제일 어려움)
		if (SelectedOC2Actor->IsA(ACookingTable::StaticClass()))
		{
			auto Table = Cast<ACookingTable>(SelectedOC2Actor);
			// 테이블과 상호작용을 시도한다.
			ACooking* Cook = Table->Interact(this);
			// 잡고있는 물건이 없고, 테이블에 올려진 물체가 있는 경우
			if (GrabbedObject == nullptr && Cook != nullptr)
			{
				Grab(Cook);
			}
			// 잡은 물건이 있는데 테이블이 비어있으면
			else if (GrabbedObject != nullptr && Cook == nullptr)
			{
				// 다른 액터에 Attach하게 되면 수동으로 Detach할 필요 X
				Table->PlaceItem(GrabbedObject);
				GrabbedObject = nullptr;
			}
			else if (GrabbedObject != nullptr && Cook != nullptr)
			{
				Table->PlaceItem(Cook);
			}
		}
	}
	else
	{
		Drop();
	}
}

void AOC2Character::Grab_Implementation(ACooking* Cook)
{
	GrabbedObject = Cook;
	GrabbedObject->AttachToChef(this);

	auto DataTable = GrabbedObject->GetIngredientDataTable();
	
	GrabbedObject->SetActorRotation(DataTable->Rotation);
	GrabbedObject->SetActorLocation(GrabComponent->GetComponentLocation());
}

void AOC2Character::Drop_Implementation()
{
	// 내가 들고 있는 물건이 있을때
	if (GrabbedObject != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Drop"));
		// 들고 있는 물체에 대해 상호작용을 실행한다. 바닥에 내려놓는다는 뜻.
		GrabbedObject->DetachFromChef(this);

		GrabbedObject->SetActorLocation(GrabComponent->GetComponentLocation());
		GrabbedObject->SetActorRotation(GetActorRotation());


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
		if (SelectedOC2Actor->IsA(AOC2CharacterTestChoppingTable::StaticClass()))
		{
			auto Table = Cast<AOC2CharacterTestChoppingTable>(SelectedOC2Actor);
			Chopping(true);
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
			GrabbedObject->DetachFromChef(this);
			GrabbedObject->SetActorLocation(GrabComponent->GetComponentLocation());
			ThrowingObject->SetThrower(this);
			ThrowingObject->SetThrowing(true);

			// 4️⃣ 던질 방향과 세기 설정
			FVector ThrowDirection = GetActorForwardVector();  // 캐릭터가 바라보는 방향
			float ThrowStrength = 1500.0f;  // 던지는 힘 조절

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

	if (SelectedOC2Actor != nullptr)
	{
		return;
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

	UE_LOG(LogTemp, Log, TEXT("%d"), HitResults.Num());

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
				SelectedOC2Actor->RestoreMaterial();
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
			SelectedOC2Actor->ApplyMaterialHighlight();
		}
		//SelectedOC2Actor->Highlight();
	}
	else
	{
		if (SelectedOC2Actor != nullptr)
		{
			//Cast<AOC2CharacterTestTable>(SelectedOC2Actor)->OffHighlight();
			SelectedOC2Actor->RestoreMaterial();
		}
		SelectedOC2Actor = nullptr;
	}
}

void AOC2Character::Dash_Implementation()
{
	if (!bIsDashing)
	{
		bIsDashing = true;
		DashTimer = 0.0f;
	}
	//FVector DashDirection = GetActorForwardVector(); // 바라보는 방향
	//LaunchCharacter(DashDirection * DashPower, true, false); // 대시 실행

}

void AOC2Character::OnRep_KnifeSet()
{
	if (bIsChopping)
	{
		GetMesh()->SetMaterial(Knife.Key, Knife.Value);
	}
	else
	{
		GetMesh()->SetMaterial(Knife.Key, TransparentMat);
	}
}

void AOC2Character::StopDash()
{
	bIsDashing = false;
	GetCharacterMovement()->StopMovementImmediately();
}

void AOC2Character::OnOverlapCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && HasAuthority())
	{
		//			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Hit");
		AIngredient* Ingredient = Cast<AIngredient>(OtherActor);
		if (Ingredient) // 캡슐과 충돌한 경우
		{
			if (Ingredient->IsThrowing() && Ingredient->GetThrower() != this)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Hit");
				Ingredient->AttachToChef(this);
				Ingredient->SetActorLocation(GrabComponent->GetComponentLocation());
				Ingredient->SetThrowing(false);
				GrabbedObject = Ingredient;
				FVector Dir = SweepResult.ImpactPoint - GetActorLocation();
				Dir.Z = 0;
				
				SetActorRotation(-GetActorRotation().Quaternion());
				
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
	DOREPLIFETIME(AOC2Character, bCanThrowing);
	DOREPLIFETIME(AOC2Character, DashTimer);
	DOREPLIFETIME(AOC2Character, bIsChopping);
	DOREPLIFETIME(AOC2Character, CharacterName);
}




