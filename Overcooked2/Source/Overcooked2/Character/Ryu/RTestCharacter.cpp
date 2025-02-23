// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ryu/RTestCharacter.h"

// Sets default values
ARTestCharacter::ARTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

