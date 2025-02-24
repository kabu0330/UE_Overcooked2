// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Park/PSWOC2Character.h"

// Sets default values
APSWOC2Character::APSWOC2Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APSWOC2Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APSWOC2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APSWOC2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

