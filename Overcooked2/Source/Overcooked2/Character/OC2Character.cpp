// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2Character.h"
#include "EnhancedInputComponent.h"

// Sets default values
AOC2Character::AOC2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOC2Character::MoveCharacter(const FVector& Value)
{
	AddMovementInput(Value);
}

// Called when the game starts or when spawned
void AOC2Character::BeginPlay()
{
	Super::BeginPlay();
	



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

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, "MoveCharacter");

}

