// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"
#include "OC2CharacterDataTable.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AOC2Character::AOC2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

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
	// юс╫ц :
	//SetCharacterHead("Alien_Green");
}

// Called every frame
void AOC2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AOC2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOC2Character::MoveCharacter);

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
		if (i == 36) continue;
		GetMesh()->SetMaterial(i, TransparentMat);
	}

	TArray<FName> SlotNames = GetMesh()->GetMaterialSlotNames();
	for (const FName& SlotName : SlotNames)
	{
		FString SlotNameStr = SlotName.ToString();

		if (SlotNameStr.StartsWith("mat_chef_hat_"))
		{
			UE_LOG(LogTemp, Log, TEXT("Filtered Material Slot: %s"), *SlotNameStr);
		}
	}

	//SetHeadMaterial(FMath::RandRange(0, HeadMaterials.Num() - 1));
}

void AOC2Character::SetCharacterHead(FString Name)
{
	if (CharacterHeadMap.Contains(Name))
	{
		FCharacterData Data = CharacterHeadMap[Name];
		GetMesh()->SetMaterial(Data.MaterialIndex, Data.Material);
	}
}


