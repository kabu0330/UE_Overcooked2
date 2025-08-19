# Overcooked! 2 언리얼 엔진 모작 프로젝트
## 플레이 영상

![bandicam 2025-07-24 20-29-53-831 (4)](https://github.com/user-attachments/assets/d293ccb3-4f28-499f-b0ea-0a6c2d93cfa4)

영상 링크 : https://youtu.be/zs3aQ8tSZ3E?si=I8BLLZopoF67xeRm

<br>

## 인게임 스크린샷
![Image](https://github.com/user-attachments/assets/18202e74-6fd8-4af3-a086-afa436a0ca45)

<br>

## 프로젝트 소개
Overcooked! 2는 최대 4명의 플레이어가 협력하여 재료를 손질하고 조리한 뒤, 접시에 담아 제출하는 협동 기반 게임입니다.  
이 게임은 각자 맡은 역할을 자연스럽게 수행하며, 손발을 맞춰 매끄럽게 요리를 완성시키는 ‘팀워크’가 재미 요소입니다.  
언리얼 엔진의 게임플레이 프레임워크에 대한 깊은 이해와 데이터 테이블 기반의 콘텐츠 관리, 다이나믹 머티리얼 활용, 팀 협업을 위한 테스트 환경 구축 등 실무적인 개발 역량을 기르는 것을 목표로 진행했습니다.  

  저는 프로젝트에서 다음과 같은 역할을 담당했습니다.
  * 요리 콘텐츠 시스템 구현
  * 데이터 테이블 설계 및 제작
  * 타이틀&로비, 스테이지 레벨 디자인 및 구현
  * 게임 내 상호작용 효과 및 간단한 파티클 이펙트 제작

<br>

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

<br>

## 주요 구현 기능
### 테스트 레벨
프로젝트의 안정성과 개발 효율을 높이기 위해 기능별 테스트를 진행할 수 있는 테스트 레벨을 구축했으며, 위젯을 통해 함수를 호출하여 정상적으로 동작하는지 확인하며 외부의 개발 속도와 관계 없이 기능을 구현해나갔습니다.
<br>

<img width="1705" height="747" alt="image" src="https://github.com/user-attachments/assets/a4b9b57a-67f5-491a-b49f-ad1646a5e99c" />

<br>

___

### 베이스 클래스 : ACooking - 공통 기능 및 인터페이스 정의
```ACooking```은 캐릭터가 잡을 수 있는 모든 요리 관련 객체들의 베이스 클래스(Base Class)입니다. <br>
중복되는 기능을 추상화하고 공통 인터페이스를 제공하여 코드의 재사용성과 유지보수성을 높이는 역할을 합니다.
<br>

* 주요 기능 및 역할
1. 공통 컴포넌트 : 모든 ```ACooking``` 파생 클래스는 ```UStaticMeshComponent```를 기본으로 가지며, 물리 시뮬레이션 및 충돌 프로필이 **Interactable**로 설정됩니다. 해당 충돌 프로필만 캐릭터와 상호작용을 수행할 수 있습니다.
2. 캐릭터 상호작용 인터페이스 : ```AttachToChef```, ```DetachFromChef``` 함수를 통해 캐릭터가 객체를 잡았을 때, 객체를 놓았을 때의 상태를 파생 클래스에서 구현하도록 했습니다.
3. 클래스 타입 제공 : 외부에서 ```Cast```를 사용하지 않고 ```GetCookingType``` 함수를 통해 파생 클래스 타입(```enum class```)을 확인할 수 있도록 편의 기능을 제공했습니다.
```cpp
class OVERCOOKED2_API ACooking : public AOC2Actor
{
public:
	UFUNCTION(NetMulticast, Reliable)
	void AttachToChef(AActor* Player);

	UFUNCTION(NetMulticast, Reliable)
	void DetachFromChef(AActor* Player);

    UFUNCTION(BlueprintCallable)
	const ECookingType GetCookingType() const 
	{
		return CookingType;
	}

	UFUNCTION(BlueprintCallable)
	bool IsCookingType(ECookingType Type) const
	{
		return Type == CookingType;
	}

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	ECookingType CookingType = ECookingType::ECT_NONE;
}
```
<br>

___

### 요리 콘텐츠 : 재료(AIngredient) - 데이터 기반 설계와 네트워크 동기화
```AIngredient``` 클래스는 게임 내 모든 요리 재료의 상태와 데이터를 표현하는 기본 단위 객체입니다. 데이터 테이블 기반의 설계와 언리얼 엔진 네트워크 프레임워크를 활용하여 콘텐츠가 확장되어도 코드 수정 없이 기능을 추가할 수 있도록 구현했으며, <br>
```SpawnActorDeferred```함수를 통해 데이터 테이블에서 재료의 타입과 상태를 먼저 설정하고 클라이언트에 복제함으로써 클라이언트에서 재료의 ```AIngredient::BeginPlay``` 호출 이전에 복제된 프로퍼티가 준비되도록 하여 네트워크 동기화를 구현했습니다.
<br>

#### 데이터 기반 설계
* 재료의 종류(```EIngredientType```), 상태(```EIngredientState```), 메시, 아이콘 등 모든 정보를 데이터 테이블로 관리했습니다.
* 이를 통해 기획자가 디자이너가 코드를 직접 수정하지 않고 새로운 재료를 추가하거나 기존 데이터를 쉽게 수정할 수 있는 확장성 높은 구조로 설계했습니다.

<p align="center">
<img alt="이미지" src="https://github.com/user-attachments/assets/62c8f003-0377-45b3-91f5-72496d8ac4e7">
</p>

<br>

#### 지연 스폰과 프로퍼티 복제 순서 보장
* ```SpawnActorDeferred```를 사용하여 액터의 ```BeginPlay```가 호출되기 전에 서버에서 재료의 핵심 데이터인 ```IngredientType```을 먼저 설정합니다.
* 이후 ```FinishSpawning```을 호출함으로써 클라이언트에서 ```BeginPlay```가 호출되는 시점에 이미 ```IngredientType``` 프로퍼티가 서버로부터 복제되어 있음을 보장하여 서버와 클라이언트 모두가 동일한 메시를 설정해 네트워크 동기화를 구현했습니다.

```cpp
// Ingredient.h - 재료 타입을 서버에서 설정하는 RPC 함수
UFUNCTION(BlueprintCallable, Reliable, Server)
void SetType(EIngredientType Type);

// Ingredient.cpp
void AIngredient::SetType_Implementation(EIngredientType Type)
{
    // Replicated 프로퍼티이므로, 서버에서 변경하면 클라이언트에 자동으로 동기화됩니다.
    IngredientType = Type;
}

void AIngredient::BeginPlay()
{
	ACooking::BeginPlay();

	InitIconWidget();
	Init(IngredientType); // 메시, 상태, 위치/회전 오프셋 등 초기 설정
}
```

<br>

#### 재료 스폰 프로세스
<p align="center">
<img alt="이미지" src="https://github.com/user-attachments/assets/57730f8b-71fd-42c9-bf8f-65388d466436">
</p>

<br>

#### 상태 관리와 데이터 정합성
* ```enum class EIngredientState```를 통해 재료의 상태를 명확하게 정의했습니다.
* 재료의 상태 변경은 서버에서만 이루어지며 ```NetMulticast```를 통해 모든 클라이언트에 변경된 상태가 복제되도록 설계하여 네트워크 데이터 정합성을 보장했습니다.
 ```cpp
 // Ingredient.h
UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
void ChangeState(EIngredientState State);

// Ingredient.cpp
void AIngredient::ChangeState_Implementation(EIngredientState State)
{
    // 데이터 테이블에서 새 상태에 맞는 메시와 오프셋 정보를 가져옵니다.
    const FIngredientCookDataRow* CookData = &CheckState(State);
    if (nullptr == CookData)
    {
        return;
    }

    CurIngredientState = State;
    StaticMeshComponent->SetStaticMesh(CookData->CookMesh);
    // ... 메시 및 위치 오프셋 조정
}
 ```
 <br>

 ___

 ### 요리 콘텐츠 : 접시(APlate) - 동적 레시피와 단일 함수 호출 시스템
 ```APlate```클래스는 여러 재료가 조합되어 새로운 결과를 만들어내는 요리 콘텐츠의 핵심 클래스입니다. <br>
 원작 게임의 "모든 요리는 접시 위에서 완성된다."는 규칙에 착안하여, 추상적인 '요리'의 개념을 물리적인 '접시'액터와 통합하여 기능을 구현했습니다.
<br>

<p align="center">
<img alt="이미지" src="https://github.com/user-attachments/assets/7166f0f9-e4ee-4fee-914e-3d210d29d58c">
</p>


#### 동적 레시피 시스템
* ```TArray<FRecipe>```에 현재 접시에 담긴 재료의 타입과 상태를 저장합니다.
* 새로운 재료가 추가될 때 마다, 이 배열과 해당 스테이지의 레시피 데이터 테이블의 리스트를 비교하여 현재 재료 조합이 유효한 레시피인지 검사하여 결과를 적용합니다.
* 이 과정을 통해 새로운 요리가 추가되어도 코드 수정 없이 데이터 테이블을 추가하는 것으로 콘텐츠를 확장할 수 있도록 설계했습니다.

<p align="center">
<img alt="이미지" src="https://github.com/user-attachments/assets/f63be4fa-bdf4-457d-9f48-dc5af267ce32">
</p>
<br>

#### 캡슐화된 단일 함수 호출
* 외부 객체는 ```Add``` 함수만 호출하면 재료의 추가, 레시피 유효성 검사, 메시 변경, 기존 재료 액터 파괴까지 모든 과정이 ```APlate``` 내부에서 처리됩니다.
* 이를 통해 객체 간의 결합도를 낮추고 요리 조합 로직을 사용자가 아닌 ```APlate```클래스가 온전히 책임지도록 설계했습니다.
```cpp
// Plate.cpp - Add 함수의 핵심 로직
void APlate::Add_Implementation(AIngredient* Ingredient)
{
    bIsCombinationSuccessful = false;
    if (false == CanPlaceOnPlate(Ingredient))
    {
        return;
    }

    // 1. 현재 재료 조합에 들어온 재료를 임시로 추가
    FRecipe Recipe;
    Recipe.IngredientType = Ingredient->GetIngredientType();
    Recipe.IngredientState = Ingredient->GetCurIngredientState();
    Ingredients.Add(Recipe);

    // 2. 데이터 테이블을 조회하여 유효한 조합인지 확인
    FPlateInitData InitData = UOC2GlobalData::GetPlateMesh(GetWorld(), Ingredients);

    if (nullptr == InitData.StaticMesh)
    {
        // 3-1. 유효하지 않으면 추가했던 재료를 다시 제거
        Ingredients.Pop();
        return;
    }
    else
    {
        // 3-2. 유효하다면 메시를 변경하고, 사용된 재료 액터를 파괴
        SetIngredinetMesh(InitData);
        if (true == HasAuthority())
        {
            Ingredient->RequestOC2ActorDestroy();
        }
        SetIngredinetTextures(InitData);
        bIsCombinationSuccessful = true;
    }
}
```

#### 레시피 조회 프로세스

<p align="center">
<img alt="이미지" src="https://github.com/user-attachments/assets/8a1d9802-6517-4e28-a894-3c11352708ab">
</p>
<br>

<br>

___

### 요리 콘텐츠 : 조리 도구(APot) - 시간 기반 상태 머신
```APot```클래스는 시간에 따라 조리 과정이 자동으로 변화하는 상태 머신 객체입니다. <br>
상태 변화 로직과 시청각적 표현을 처리하는 로직을 분리하고, 여러 컴포넌트를 활용하여 사용자에게 복합적인 피드백을 제공하는 시스템을 구축했습니다.
<br>

#### 시간 기반 상태 머신
* ```enum class EPotStat```를 통해 각 상태(Idle, Heating, Cooking, Overcooked 등)을 정의하고
* ```Tick```함수 내에서 호출되는 ```Cook```함수가 시간의 경과를 누적하며, 미리 정의된 시간에 도달하면 자동으로 다음 상태로 전이합니다.
```cpp
// Pot.cpp - 시간 경과에 따른 상태 변화 로직
void APot::Cook(float DeltaTime)
{
    if (false == IsBoiling()) return;

    TimeElapsed += DeltaTime;

    // 예시: TimeToCook(12초)이 경과하면 BOILING 상태에서 COOKED 상태로 변경
    ChangeState(EPotState::BOILING, EPotState::COOKED, TimeToCook);
    ChangeState(EPotState::COOKED, EPotState::COOKED_WARNING, TimeToWarning);
    // ...
}

void APot::ChangeState(EPotState CurState, EPotState NextState, float TransitionTime)
{
    if (CurState == PotState && TransitionTime < TimeElapsed)
    {
        PotState = NextState;
    }
}
```
<br>

#### 상태와 표현의 분리
* 상태를 변화시키는 로직(```Cook```)과 상태가 변경되었을 때 시청각 표현을 처리하는 로직(```SetAction```)을 명확히 분리하여 가독성과 유지보수성을 고려하여 구조를 설계했습니다.
```cpp
// Pot.cpp - 상태 변화 감지 및 표현 처리
void APot::SetAction_Implementation()
{
    // 이전 상태와 현재 상태가 동일하면 아무 처리도 하지 않음
    if (PrevPotState == PotState)
    {
        return;
    }
    PrevPotState = PotState;

    switch (PotState)
    {
        case EPotState::COOKED:
            bIsCooked = true;
            TargetColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
            StatusWidgetComponent->bHiddenInGame = false; // 완료 아이콘 표시
            SetNiagaraAsset(TEXT("Steam"));               // 증기 이펙트 재생
            PlaySound(ImCookedEffectAudioComponent, false); // 완료 효과음 재생
            break;
        // ... 다른 상태에 대한 처리
    }
}
```
<br>

#### 다중 컴포넌트를 활용한 복합적 피드백
* 단일 액터에 여러 컴포넌트를 조합하여 사용자에게 풍부한 정보를 제공했습니다.
1. UGaugeTextureWidget   : 요리 진행률을 보여주는 게이지 바
2. UCookStatusWidget     : 조리 완료 및 타기 직전 경고 아이콘 표시
3. UIngredientIconWidget : 냄비에 들어간 재료 아이콘 표시
4. UNiagaraComponent     : 증기 및 연기 이펙트
5. UAudioComponent       : 끓는 소리, 경고음 등 각종 효과음

<p align="center">
<img alt="이미지" src="https://github.com/user-attachments/assets/7be87caa-59ef-4436-b114-c6a2eb9935c8">
</p>




