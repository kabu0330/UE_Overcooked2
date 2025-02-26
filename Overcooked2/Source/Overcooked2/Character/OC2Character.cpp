// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"

// Sets default values
AOC2Character::AOC2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bUseControllerRotationYaw = false;

	GrabComponent = CreateDefaultSubobject<USceneComponent>("GrabPosition");
	GrabComponent->SetupAttachment(RootComponent);
}

void AOC2Character::MoveCharacter(const FInputActionValue& Value)
{
	FVector MovementInput = Value.Get<FVector>();

	MovementInput.Normalize();

	AddMovementInput(MovementInput);
	
	FQuat ActorRot =  GetActorForwardVector().Rotation().Quaternion();
	FQuat TargetRot = FRotationMatrix::MakeFromX(MovementInput).Rotator().Quaternion();

	SetActorRotation(FQuat::Slerp(ActorRot, TargetRot, Alpha).Rotator());
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
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &AOC2Character::CheckInteract);
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
		auto Temp = Cast<AOC2CharacterTestTable>(ClosestActor);
		if (CurHighlightedTable != nullptr && Temp != CurHighlightedTable)
		{
			CurHighlightedTable->OffHighlight();
		}
		CurHighlightedTable = Temp;
		CurHighlightedTable->Highlight();
	}
	else
	{
		if (CurHighlightedTable != nullptr)
		{
			CurHighlightedTable->OffHighlight();
		}
	}
}

void AOC2Character::Dash()
{
	LaunchCharacter(GetActorForwardVector()*100.0f, false, false);
}

void AOC2Character::SetCharacterHead(FString Name)
{
	if (CharacterHeadMap.Contains(Name))
	{
		FCharacterData Data = CharacterHeadMap[Name];
		GetMesh()->SetMaterial(Data.MaterialIndex, Data.Material);
	}
}


