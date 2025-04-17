// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/ServingTable.h"
#include "LevelContent/Cook/Plate.h"
#include "LevelContent/Table/NonTable/PlateSpawner.h"

#include "Global/OC2Global.h"
#include "Global/Data/OC2GlobalData.h"
#include "Global/GameMode/OC2GameMode.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include <Global/Component/TimeEventComponent.h>
#include "CoreMinimal.h"


AServingTable::AServingTable()
{
	//bReplicates = true;

	RingNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RingNiagaraComponent"));
	RingNiagaraComponent->SetupAttachment(RootComponent);
	RingNiagaraComponent->SetAutoActivate(false);

	FlashNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlashNiagaraComponent"));
	FlashNiagaraComponent->SetupAttachment(RootComponent);
	FlashNiagaraComponent->SetAutoActivate(false);

	ConfettiNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ConfettiNiagaraComponent"));
	ConfettiNiagaraComponent->SetupAttachment(RootComponent);
	ConfettiNiagaraComponent->SetAutoActivate(false);

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent"));
	
}

void AServingTable::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACookingTable> It(GetWorld()); It; ++It)
	{
		ACookingTable* PrepTableActor = *It;
		if (PrepTableActor->Tags.Contains("PlateSpawner"))
		{
			PlateSpawner = Cast<APlateSpawner>(PrepTableActor);
		}
	}

	SoundEffect = UOC2GlobalData::GetTableBaseSound(GetWorld(), "ServiceBell");

	InitNiagara();
}

void AServingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ShowServingEffet();
}

void AServingTable::PlaceItem(ACooking* ReceivedCooking)
{
	if (nullptr == ReceivedCooking)
	{
		return;
	}

	if (ECookingType::ECT_PLATE == ReceivedCooking->GetCookingType())
	{
	}
	else
	{
		return;
	}

	Super::PlaceItem(ReceivedCooking);

	CookingPtr->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CookingPtr->SetActorLocation(CookingPtr->GetActorLocation() + FVector(-10, 0, 10));

	UOC2Global::SubmitPlate(GetWorld(), ReceivedCooking);

	//효과음
	PlaySoundEffect();

	ShowEffect();

	//Server_SpawnPlateInPlateSpawner();

	CookingPtr = nullptr;
}

void AServingTable::Server_SpawnPlateInPlateSpawner_Implementation()
{
	auto GameMode = Cast<AOC2GameMode>(GetWorld()->GetAuthGameMode());
	APlate* Plate = nullptr;
	if (GameMode)
	{
		Plate = GameMode->SpawnPlateActor(EPlateState::DIRTY);
	}
	//Plate->AttachToChef(PlateSpawner);
	//PlateSpawner->PlaceItem(Plate);
	//PlateSpawner->SetPlate(Plate);
	Plate->Multicast_SubmitPlate();
	//PlateSpawner->AddPlate(int Num1);

}

void AServingTable::PlaySoundEffect_Implementation()
{
	if (nullptr != SoundEffect)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundEffect);
	}
}

void AServingTable::InitNiagara()
{
	if (nullptr != RingNiagaraComponent)
	{
		FString ResourceName = TEXT("RingContract");
		FResourceNiagaraDataRow NiagaraDataRow = UOC2GlobalData::GetResourceNiagaraDataRow(GetWorld(), *ResourceName);
		if (nullptr != NiagaraDataRow.NiagaraSystem)
		{
			UNiagaraSystem* FX = NiagaraDataRow.NiagaraSystem;
			RingNiagaraComponent->SetAsset(FX);

			RingNiagaraComponent->SetRelativeLocation(NiagaraDataRow.Location);
			RingNiagaraComponent->SetRelativeRotation(NiagaraDataRow.Rotation);
		}

		if (nullptr != RingNiagaraComponent)
		{
			// OnSystemFinished : 시스템 재생 완료 시 호출되는 델리게이트(delegate)
			RingNiagaraComponent->OnSystemFinished.AddDynamic(
				this,
				&AServingTable::HandleNiagaraFinished
			);
		}
	}

	if (nullptr != FlashNiagaraComponent)
	{
		FString ResourceName = TEXT("RingExpand");
		FResourceNiagaraDataRow NiagaraDataRow = UOC2GlobalData::GetResourceNiagaraDataRow(GetWorld(), *ResourceName);
		if (nullptr != NiagaraDataRow.NiagaraSystem)
		{
			UNiagaraSystem* FX = NiagaraDataRow.NiagaraSystem;
			FlashNiagaraComponent->SetAsset(FX);

			FlashNiagaraComponent->SetRelativeLocation(NiagaraDataRow.Location);
			FlashNiagaraComponent->SetRelativeRotation(NiagaraDataRow.Rotation);
		}

		if (nullptr != FlashNiagaraComponent)
		{
			// OnSystemFinished : 시스템 재생 완료 시 호출되는 델리게이트(delegate)
			FlashNiagaraComponent->OnSystemFinished.AddDynamic(
				this,
				&AServingTable::HandleNiagaraFinished
			);
		}
	}

	if (nullptr != ConfettiNiagaraComponent)
	{
		FString ResourceName = TEXT("Confetti");
		FResourceNiagaraDataRow NiagaraDataRow = UOC2GlobalData::GetResourceNiagaraDataRow(GetWorld(), *ResourceName);
		if (nullptr != NiagaraDataRow.NiagaraSystem)
		{
			UNiagaraSystem* FX = NiagaraDataRow.NiagaraSystem;
			ConfettiNiagaraComponent->SetAsset(FX);

			ConfettiNiagaraComponent->SetRelativeLocation(NiagaraDataRow.Location);
			ConfettiNiagaraComponent->SetRelativeRotation(NiagaraDataRow.Rotation);
		}

		if (nullptr != ConfettiNiagaraComponent)
		{
			// OnSystemFinished : 시스템 재생 완료 시 호출되는 델리게이트(delegate)
			ConfettiNiagaraComponent->OnSystemFinished.AddDynamic(
				this,
				&AServingTable::HandleNiagaraFinished
			);
		}
	}
}

void AServingTable::SetNiagara(class UNiagaraComponent* NiagaraComponent, bool IsActivate)
{
	if (nullptr != NiagaraComponent->GetAsset())
	{
		if (true == IsActivate)
		{
			NiagaraComponent->Activate(true);
		}
		else
		{
			NiagaraComponent->Deactivate();
		}
	}
}

void AServingTable::ShowServingEffet()
{
	if (false == bCanEffet)
	{
		return;
	}

	bCanEffet = false;

	SetNiagara(RingNiagaraComponent, true);
	TimeEventComponent->AddEndEvent(0.2f, [this]()
		{
			SetNiagara(FlashNiagaraComponent, true);

		});		
	TimeEventComponent->AddEndEvent(0.8f, [this]()
		{
			SetNiagara(ConfettiNiagaraComponent, true);

		});
	
}

void AServingTable::ShowEffect_Implementation()
{
	// 이펙트가 이미 출력중이면 또 출력 안 한다.
	if (false == RingNiagaraComponent->IsActive())
	{
		bCanEffet = true;
		NetMulticast_CanEffect(); // 클라 이펙트 출력 허용

		if (true == HasAuthority())
		{
			ShowServingEffet();
			NetMuticast_ShowEffect();
		}
	}
}

void AServingTable::NetMuticast_ShowEffect_Implementation()
{
	if (true == HasAuthority())
	{
		return;
	}
	ShowServingEffet();
}

void AServingTable::HandleNiagaraFinished(UNiagaraComponent* NiagaraComponent)
{
	SetNiagara(NiagaraComponent, false);
}

void AServingTable::NetMulticast_CanEffect_Implementation()
{
	bCanEffet = true;
}

//void AServingTable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//}

