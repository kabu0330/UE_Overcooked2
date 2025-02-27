// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"
#include "OC2CharacterTestTable.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AOC2Character::AOC2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	//bUseControllerRotationYaw = false;

	GrabComponent = CreateDefaultSubobject<USceneComponent>("GrabPosition");
	GrabComponent->SetupAttachment(RootComponent);
}

void AOC2Character::MoveCharacter(const FInputActionValue& Value)
{
	FVector MovementInput = Value.Get<FVector>();


	MovementInput.Normalize();

	AddMovementInput(MovementInput);
	float CurrentYaw = GetActorRotation().Yaw;
	float TargetYaw = MovementInput.Rotation().Yaw;

	float InterpSpeed = 10.0f; // 회전 속도 조절 가능
	float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

	float YawDelta = FMath::FindDeltaAngleDegrees(CurrentYaw, NewYaw);

	AddControllerYawInput(YawDelta);

	//FQuat ActorRot =  GetActorForwardVector().Rotation().Quaternion();
	//FQuat TargetRot = FRotationMatrix::MakeFromX(MovementInput).Rotator().Quaternion();

	//SetActorRotation(FQuat::Slerp(ActorRot, TargetRot, Alpha).Rotator());
}

// Called when the game starts or when spawned
void AOC2Character::BeginPlay()
{
	Super::BeginPlay();

	InitMesh();
	// 임시 :
	//SetCharacterHead("Alien_Green");
}

// Called every frame
void AOC2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckInteract();

	DrawDebugSphere(GetWorld(), GrabComponent->GetComponentLocation(), TraceRadius, 20, FColor::Green, false, 0.0f);
}

// Called to bind functionality to input
void AOC2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOC2Character::MoveCharacter);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &AOC2Character::Interact);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AOC2Character::Dash);
}

void AOC2Character::InitMesh()
{
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	//0 is ChefBody
	for (int32 i = 1; i < Materials.Num(); i++)
	{
		FString Name = Materials[i]->GetName();
		if (Name.Contains("ChefHead"))
		{
			FString Key = Name.RightChop(15);
			CharacterHeadMap.Add(Key, FCharacterData(Materials[i], i));
		}
		// hat index
		if (i == 36) continue;
		GetMesh()->SetMaterial(i, TransparentMat);
	}

	//SetHeadMaterial(FMath::RandRange(0, HeadMaterials.Num() - 1));
}

//상호작용 : Space Key
void AOC2Character::Interact()
{
	// 만약 지금 상호작용을 시도할 수 있는 개체가 있으면
	if (SelectedOC2Actor != nullptr)
	{
		//만약 이 액터가 재료나 요리 접시인 경우
		if (SelectedOC2Actor->IsA(ACooking::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("Hold"));
			// 내가 들고있는 액터에 저장한다.
			Grab(Cast<ACooking>(SelectedOC2Actor));

		}
		//만약 이 액터가 테이블인 경우
		if (SelectedOC2Actor->IsA(ACookingTable::StaticClass()))
		{
			auto Table = Cast<ACookingTable>(SelectedOC2Actor);
			Grab(Table->Interact(this));
		}
	}
	else
	{
		Drop();
	}
}

void AOC2Character::Grab(ACooking* Cook)
{
	if (GrabbedObject == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Grab"));
		GrabbedObject = Cook;
		GrabbedObject->AttachToComponent(GrabComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Cast<UPrimitiveComponent>(GrabbedObject->GetRootComponent())->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AOC2Character::Drop()
{
	// 주위에 상호작용할 수 있는 개체가 아무것도 없으면
	if (GrabbedObject)
	{
		UE_LOG(LogTemp, Log, TEXT("Drop"));
		// 잡은 물체에 대해 상호작용을 실행한다. 바닥에 내려놓는다는 뜻.
		GrabbedObject->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		GrabbedObject->SetActorLocation(GrabComponent->GetComponentLocation());
		Cast<UPrimitiveComponent>(GrabbedObject->GetRootComponent())->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Cast<UPrimitiveComponent>(GrabbedObject->GetRootComponent())->SetCollisionProfileName(TEXT("Interactable"));

		GrabbedObject = nullptr;
	}
}

void AOC2Character::CheckInteract()
{
	if (!GrabComponent) return; // SceneComponent가 유효한지 확인

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

		AActor* ClosestActor = HitResults[0].GetActor();
		// maybe Interactable.
		auto Temp = Cast<AOC2Actor>(ClosestActor);
		if (SelectedOC2Actor!= nullptr && Temp != SelectedOC2Actor)
		{
			//Cast<AOC2CharacterTestTable>(SelectedOC2Actor)->OffHighlight();
			//auto Mesh = SelectedOC2Actor->FindComponentByClass<UStaticMeshComponent>();
			//UMaterialInstanceDynamic* DynamicMat = Mesh->CreateDynamicMaterialInstance(0);
			//if (DynamicMat)
			//{
			//	DynamicMat->SetScalarParameterValue(TEXT("Brightness"), 2.0f);
			//}
		}
		SelectedOC2Actor = Temp;
		//Cast<AOC2CharacterTestTable>(SelectedOC2Actor)->Highlight();
		//SelectedOC2Actor->Highlight();
	}
	else
	{
		SelectedOC2Actor = nullptr;

		if (SelectedOC2Actor != nullptr)
		{
			//Cast<AOC2CharacterTestTable>(SelectedOC2Actor)->OffHighlight();
			//SelectedOC2Actor->OffHighlight();
		}
	}
}

void AOC2Character::Dash()
{
	if (GetCharacterMovement()->IsFalling()) return; // 공중에서는 대시 불가

	FVector DashDirection = GetActorForwardVector(); // 바라보는 방향
	LaunchCharacter(DashDirection * DashPower, true, false); // 대시 실행

	// 일정 시간이 지나면 멈추도록 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &AOC2Character::StopDash, DashDuration, false, DashDuration);
}

void AOC2Character::StopDash()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void AOC2Character::SetCharacterHead(FString Name)
{
	if (CharacterHeadMap.Contains(Name))
	{
		FCharacterData Data = CharacterHeadMap[Name];
		GetMesh()->SetMaterial(Data.MaterialIndex, Data.Material);
	}
}


