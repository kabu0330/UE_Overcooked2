// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/BusCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABusCharacter::ABusCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetBoxExtent(FVector(62.0f, 32.f, 32.f));
	BoxComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetShouldUpdatePhysicsVolume(true);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->bDynamicObstacle = true;
	RootComponent = BoxComponent;

	CharacterMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharMoveComp"));	// TODO
	if (CharacterMovement)
	{
		CharacterMovement->UpdatedComponent = BoxComponent;
	}
}

// Called when the game starts or when spawned
void ABusCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABusCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABusCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABusCharacter::OnMove(const FVector2D& _Vec)
{
	AddMovementInput(FVector(_Vec.X, _Vec.Y, 0.f));
}