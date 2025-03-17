// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Component/OrderManageComponent.h"

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOrderManageComponent::UOrderManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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


// Called when the game starts
void UOrderManageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOrderManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
