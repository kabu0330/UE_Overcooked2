// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Cooking/CaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/KismetRenderingLibrary.h"

UCaptureComponent2D::UCaptureComponent2D()
{

	//PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_LegacySceneCapture;
	ProjectionType = ECameraProjectionMode::Orthographic;
	CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	//CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR; 

	OrthoWidth = 180.0f;

	TextureTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
	if (TextureTarget)
	{
		TextureTarget->InitAutoFormat(256, 256);
		//TextureTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f; 
		//TextureTarget->ClearColor = FLinearColor(0, 0, 0, 0);
	}
}

void UCaptureComponent2D::BeginPlay()
{
	Super::BeginPlay();
	CaptureScene();
}

void UCaptureComponent2D::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 매 프레임마다 캡처
	CaptureScene();
}


void UCaptureComponent2D::ShowOnlyActor(AActor* Character)
{
	if (Character != nullptr)
	{
		ShowOnlyActors.Add(Character);
		CaptureScene();
	}
}


