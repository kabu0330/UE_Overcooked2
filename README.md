# Overcooked! 2 언리얼 엔진 모작 프로젝트

![Image](https://github.com/user-attachments/assets/22a88ae9-27e3-47ae-b79f-86288e5b93b3)

## 플레이 영상

![bandicam 2025-07-24 20-29-53-831 (4)](https://github.com/user-attachments/assets/d293ccb3-4f28-499f-b0ea-0a6c2d93cfa4)

영상 링크 : https://youtu.be/zs3aQ8tSZ3E?si=I8BLLZopoF67xeRm
____
## 프로젝트 소개
Overcooked! 2는 최대 4명의 플레이어가 협력하여 재료를 손질하고 조리한 뒤, 접시에 담아 제출하는 협동 기반 게임입니다.  
이 게임은 각자 맡은 역할을 자연스럽게 수행하며, 손발을 맞춰 매끄럽게 요리를 완성시키는 ‘팀워크’가 재미 요소입니다.  
언리얼 엔진의 게임플레이 프레임워크에 대한 깊은 이해와 데이터 테이블 기반의 콘텐츠 관리, 다이나믹 머티리얼 활용, 팀 협업을 위한 테스트 환경 구축 등 실무적인 개발 역량을 기르는 것을 목표로 진행했습니다.  

  저는 해당 프로젝트에서 레벨, 그 중에서도 요리 컨텐츠와 레벨 디자인을 맡았습니다.   
  캐릭터와 상호작용하며 재료가 하나의 완성된 요리로 만들어지는 과정을 구현하는 데 가장 많은 고민을 했습니다.  
  팀과의 소통을 위해 노션과 디스코드, 수도 코드를 통해 아이디어를 공유하며 문제를 해결했습니다.

## 인게임 스크린샷
![Image](https://github.com/user-attachments/assets/18202e74-6fd8-4af3-a086-afa436a0ca45)



## 📌 팀원 및 역할  

| 👤 팀원   | 🎭 역할       | 🔗 GitHub 링크 |
|----------|------------|--------------|
| 🧑‍🎨 박석원 | 캐릭터      | [Seokwon-Park](https://github.com/Seokwon-Park) |
| 🏗️ 류성민  | 레벨        | [kabu0330](https://github.com/kabu0330) |
| 🏗️ 이수환  | 레벨        | [dfgh2727](https://github.com/dfgh2727) |
| 🎨 강민정  | UI         | [minjeong0917](https://github.com/minjeong0917) |
| 🌍 전혜주  | 월드        | [JeonHyeJu](https://github.com/JeonHyeJu) |
| 🌐 김용일  | 글로벌      | [yongilkim01](https://github.com/yongilkim01) |


## 📑 공용 문서  
- [Notion](https://www.notion.so/19ea97d70b81808f9b0ed999d1a88230?pvs=4)



## 주요 기능 구현 상세

### 1. 데이터 테이블 설정
재료, 요리 조합, 주문을 구현하기 위하여 데이터 테이블을 활용했습니다.  
![Image](https://github.com/user-attachments/assets/74f843ae-52c3-4303-b75f-575836b661d1)


![Image](https://github.com/user-attachments/assets/1e31fc00-8b09-4d1a-8ba7-a240afd1fcd5)


재료는 종류(Type)와 상태(State)로 정의했습니다.


ex). 물고기(Type)를 손질하다(State). / 밥(Type)을 짓다(State).



'FIngredient Data Row'에서 재료와 관련된 데이터로 다음과 같이 설정했습니다.


|타입|목적|
|--|--|
|`UTexture2D*`|아이콘 텍스처|
|`EIngredientType`|재료의 종류|
|`UStatticMesh*`|재료 메시 리소스|
|`Location`|위치 조정|
|`Rotation`|회전 조정|
|`Scale`|크기 조정|
|`TArray<FIngredientCookDataRow>`|재료가 손질되어 바뀔 수 있는 모든 상태들|

![Image](https://github.com/user-attachments/assets/78cff9ec-8c5f-4485-b08f-91b6394fd21d)


___

### 2. 재료
재료는 메시를 가지고 있지 않습니다.  
재료는 타입(enum class)을 이용하여 데이터 테이블에 있는 메시를 로드하는 방식으로 구성되어 있습니다.  
캐릭터의 상호작용으로 월드에 재료가 스폰될 때, 재료는 자신이 어떤 메시로 보여질 것인지를 알아야합니다.  
그래서 SpawnActorDeferred<>()를 통해 지연 호출 전략을 사용하여  
그 사이에 재료의 타입을 결정하고(SetType()) BeginPlay에서 메시를 세팅하는 방법을 사용했습니다.  

```
AIngredient* USpawnManageComponent::SpawnIngredientActor(TSubclassOf<AIngredient> IngredientToSpawn, EIngredientType IngredientType)
{
	FTransform IngredientTransform;
	AIngredient* IngredientActor = GetWorld()->SpawnActorDeferred<AIngredient>(IngredientToSpawn, IngredientTransform);
	
	if (nullptr == IngredientActor)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Ingredient actor spawn failed"));

		return nullptr;
	}

	IngredientActor->SetType(IngredientType); // 여기서 타입을 결정

	FVector IngredientLocation = FVector(0.0f, 0.0f, 100.0f);
	IngredientTransform.SetLocation(IngredientLocation);

	IngredientActor->FinishSpawning(IngredientTransform); // 이후 BeginPlay에서 서버/클라 동일한 메시 설정

	return IngredientActor;
}
```


___


### 3. 아이콘 

접시 위에 놓인 여러 재료 아이콘을 3D 월드 상에 표시해야 했습니다.

처음에는 월드 공간(World Space)에서 빌보드(Billboard) 컴포넌트를 사용해 텍스처를 직접 렌더링하는 방식을 시도했습니다.  
하지만 이 방법은 카메라의 각도와 거리에 따라 아이콘 간의 간격이 비정상적으로 보이거나 겹치는 등 원근 왜곡(Perspective Distortion) 문제가 심각했습니다.   
또한, 아이콘 개수에 따라 간격을 동적으로 계산하는 로직도 복잡했습니다.

이 문제를 해결하기 위해, 3D 월드 공간에 UI 위젯을 렌더링할 수 있는 '위젯 컴포넌트' 를 도입했습니다.   
위젯 컴포넌트는 스크린 공간(Screen Space)처럼 작동하는 UMG(Unreal Motion Graphics)의 강력한 레이아웃 시스템을 그대로 사용할 수 있어, 아이콘 개수에 따른 위치와 간격 조절을 매우 쉽게 처리할 수 있었습니다.  
이를 통해 원근 왜곡 문제없이 항상 일정한 간격의 아이콘을 안정적으로 표시하는 데 성공했습니다.

<img width="2380" height="1191" alt="image" src="https://github.com/user-attachments/assets/cf6e2ec7-9f5c-4596-8d10-0ac84195a843" />


```
void UPlateIconWidget::SetIngredientTextures(const TArray<UTexture2D*>& InTextures)
{
    TArray<UImage*> Slots = { ImageSlot1, ImageSlot2, ImageSlot3 };

    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (Slots[i])
        {
            if (i < InTextures.Num() && nullptr != InTextures[i])
            {
                Slots[i]->SetBrushFromTexture(InTextures[i]);
                Slots[i]->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                Slots[i]->SetVisibility(ESlateVisibility::Collapsed); // 없는 건 숨김
            }
        }
    }

	if (1 == InTextures.Num())
	{
		Slots[0]->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
	else if (2 == InTextures.Num())
	{
		for (int32 i = 0; i < Slots.Num(); ++i)
		{
			if (Slots[i] && Slots[i]->GetVisibility() == ESlateVisibility::Visible)
			{
				Slots[i]->SetRenderTranslation(FVector2D::ZeroVector);
			}
		}
	}
	else
	{
		const FVector2D Offset(0.0f, -250.0f); // Y축 아래로
		// 3개 다 있을 경우엔 위치 초기화 (위로 다시 올림)
		for (int32 i = 0; i < Slots.Num(); ++i)
		{
			if (Slots[i])
			{
				Slots[i]->SetRenderTranslation(FVector2D::ZeroVector);
				Slots[i]->SetRenderTranslation(Offset);
			}
		}
	}
}
```

___
### 4. 머티리얼 인스턴스 다이나믹

스테이지 레벨에 존재하는 대부분의 오브젝트는 모두 베이스 머티리얼을 상속받은 머티리얼 인스턴스로 구성했습니다.  
제작 초기에는 머티리얼 인스턴스를 이용하여 작업하는 것이 업계에서 선호되는 방식으로만 알고 있었으나,   
작업 중반부에 원작에서 캐릭터가 상호작용이 가능한 오브젝트가 근처에 있으면 강조되는 효과를 구현해야하는 요청을 받았을 때,  
빠르게 해당 문제를 해결할 방법으로 쓰였습니다.  

<img width="2413" height="1204" alt="image" src="https://github.com/user-attachments/assets/beb19f1a-7008-40b6-b365-01c877271383" />

```
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	if (true == MeshComponents.IsEmpty())
	{
		return;
	}

	int Count = 0;

	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (nullptr == Mesh)
		{
			continue;
		}
		if (nullptr != Cast<UWidgetComponent>(Mesh))
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			UMaterialInterface* Material = Mesh->GetMaterials()[i];
			if (nullptr != Material)
			{
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
				if (nullptr != DynamicMaterial)
				{
					float Temp;
					Mesh->GetMaterials()[i]->GetScalarParameterValue(FName("DiffuseAdd"), Temp);
					DiffuseColorMapWeights.Add(Temp); // 원래 데이터로 되돌아가기 위해 저장
					Mesh->SetMaterial(i, DynamicMaterial);
				}
			}
		}
	}
```
  비록 해당 클래스를 상속받는 모든 대상이 머티리얼 인스턴스 다이나믹을 만든다는 점에서 약간의 성능 부담이 있을 수 있지만,  
  실제로 프레임 드랍이 발생할 수준은 아니었고 무엇보다도 기능 추가 요청에 대한 개발 시간을 크게 단축할 수 있었습니다.


___

### 5. 테스트 레벨 
요리라는 컨텐츠를 구현하는데 있어 가장 어려웠던 부분 중 하나로 어떻게 기능을 테스트하는 것이 좋을지에 대한 고민이었습니다.  
1인 개발을 할 때는 모든 권한이 저에게 있었기 때문에 캐릭터를 활용해서 테스트를 하면 되는 간단한 문제이지만,   
캐릭터가 존재하지 않는 상황 혹은 캐릭터의 기능 구현이 완전하지 않은 상태에서 수동적으로 움직일 수 밖에 없는 대상을 어떻게 테스트하면 좋을지 생각했습니다.  
그 방법 중 하나로, 함수의 호출만을 담당할 별도의 레벨과 위젯을 구현하는 것이었습니다.

함수의 호출로 그 결과를 확인하고, PlayerState를 이용해 RPC Server함수를 호출하는 방식으로 캐릭터를 우회해서 동기화를 테스트했습니다.


<img width="1705" height="747" alt="image" src="https://github.com/user-attachments/assets/a4b9b57a-67f5-491a-b49f-ad1646a5e99c" />


