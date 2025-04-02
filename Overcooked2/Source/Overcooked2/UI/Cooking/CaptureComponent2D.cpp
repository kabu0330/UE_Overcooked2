// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Cooking/CaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/KismetRenderingLibrary.h"

UCaptureComponent2D::UCaptureComponent2D()
{

	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	ProjectionType = ECameraProjectionMode::Orthographic;
	//CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR; // 투명 배경을 포함하려면 HDR 사용

	OrthoWidth = 180.0f;

	TextureTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
	if (TextureTarget)
	{
		TextureTarget->InitAutoFormat(256, 256);
		TextureTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f; 
		TextureTarget->ClearColor = FLinearColor(0, 0, 0, 0);
	}
}

void UCaptureComponent2D::BeginPlay()
{
	Super::BeginPlay();

}

void UCaptureComponent2D::ShowOnlyActor(AActor* Character)
{
	if (Character != nullptr)
	{
		ShowOnlyActors.Add(Character);
		CaptureScene();
	}
}


