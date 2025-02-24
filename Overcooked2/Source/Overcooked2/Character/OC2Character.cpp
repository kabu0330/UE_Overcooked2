// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"
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

	//InitMesh();
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
	for (int i = 0; i < Materials.Num(); i++)
	{
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
}

