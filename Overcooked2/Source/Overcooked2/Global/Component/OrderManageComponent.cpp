// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Component/OrderManageComponent.h"

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Kismet/GameplayStatics.h"

UOrderManageComponent::UOrderManageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

int UOrderManageComponent::FindOrderIndex(FOrder& Order)
{
	for (int i = 0; i < OrderList.Num(); i++)
	{
		if (Order.OrderTexutre == OrderList[i].OrderTexutre)
		{
			return i;
		}
	}

	return -1;
}

void UOrderManageComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UOrderManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UOrderManageComponent::Multicast_CompleteOrder_Implementation(FOrder Order)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			int OrderIndex = FindOrderIndex(Order);

			if (-1 != OrderIndex)
			{
				CookingHUD->CookWidget->OrderComplete(OrderIndex);
			}
		}
	}
}

void UOrderManageComponent::Multicast_CreateNewOrder_Implementation(FOrder Order)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			FOrder CurOrder = Order;
			OrderList.Push(Order);
			CookingHUD->CookWidget->CreateNewOrder(Order);
		}
	}
}

void UOrderManageComponent::Multicast_BlinkOrderUI_Implementation()
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->WrongOrder();
		}
	}
}
