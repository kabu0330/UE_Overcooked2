// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldPlayer.h"

// Sets default values
AWorldPlayer::AWorldPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;*/
}

// Called when the game starts or when spawned
void AWorldPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWorldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWorldPlayer::InputMove(const FVector2D& _Vec)
{
	AddMovementInput(FVector(_Vec.X * 0.5f, _Vec.Y * 0.5f, 0.f));
}