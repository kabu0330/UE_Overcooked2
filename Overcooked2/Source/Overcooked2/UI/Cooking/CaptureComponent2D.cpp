// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Cooking/CaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/KismetRenderingLibrary.h"

UCaptureComponent2D::UCaptureComponent2D()
{

	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	ProjectionType = ECameraProjectionMode::Orthographic;
	CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	OrthoWidth = 180.0f;
	bCaptureEveryFrame = false;
	bCaptureOnMovement = true;

	ShowFlags.SetDynamicShadows(false);
	ShowFlags.SetAmbientOcclusion(false);
	ShowFlags.SetGlobalIllumination(false);

	//TextureTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("C++RenderTarget"));
	//if (TextureTarget)
	//{
	//	TextureTarget->InitAutoFormat(256, 256);
	//}
}

void UCaptureComponent2D::BeginPlay()
{
	Super::BeginPlay();
	CaptureScene();
		

}

void UCaptureComponent2D::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


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


