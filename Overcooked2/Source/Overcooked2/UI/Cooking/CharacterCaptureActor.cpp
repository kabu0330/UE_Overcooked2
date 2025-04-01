// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/CharacterCaptureActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Character/OC2Character.h"

// Sets default values
ACharacterCaptureActor::ACharacterCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	//SceneCaptureComponent2D->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_LegacySceneCapture;
	SceneCaptureComponent2D->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	SceneCaptureComponent2D->OrthoWidth = 180.0f;

	SceneCaptureComponent2D->SetupAttachment(DefaultScene);

	RootComponent = DefaultScene;
}

// Called when the game starts or when spawned
void ACharacterCaptureActor::BeginPlay()
{
	Super::BeginPlay();

	if (TargetCharacter)
	{
		SceneCaptureComponent2D->ShowOnlyActors.Add(TargetCharacter);
	}
}

// Called every frame
void ACharacterCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

