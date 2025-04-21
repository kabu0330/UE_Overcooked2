// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Global/GameMode/OC2GameMode.h"

#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Const.h"

#include "CookingGameMode.generated.h"

class UOrderManageComponent;
class USpawnManageComponent;
class AIngredient;
class APlate;
class AOC2Actor;
class ACookingGameState;
class ACookingTable;
class ASpawnManager;
class AStageManager;

/**
 * 요리 게임 모드 클래스 (ACookingGameMode)
 *
 * Overcooked2 스타일의 멀티플레이 요리 게임을 관리하는 게임 모드 클래스입니다.
 * 게임의 상태 전환, 플레이어 초기화, 주문 관리 등을 수행합니다.
 */
UCLASS()
class OVERCOOKED2_API ACookingGameMode : public AOC2GameMode
{
	GENERATED_BODY()

public:
	/** 생성자 - 기본 설정을 초기화합니다. */
	ACookingGameMode();

protected:
	/**
	 * 게임이 시작될 때 호출되는 함수
	 * 기본적인 게임 상태와 환경을 초기화합니다.
	 */
	virtual void BeginPlay() override;

	/**
	 * 매 프레임마다 호출되는 함수
	 * @param DeltaTime 지난 프레임과 현재 프레임 사이의 시간 간격 (초 단위)
	 */
	virtual void Tick(float DeltaTime) override;

public:
	void AddPlate(APlate* Plate);
	APlate* GetPlate();

public:
	/** 상태 함수 */
	void EntryStay();
	void Stay(float DeltaTime);
	void EntryStage();
	void Stage(float DeltaTime);
	void EntryScore();
	void TimeUp(float DeltaTime);
	void EntryTimeUp();
	void Score(float DeltaTime);
	void ChangeState(ECookingGameModeState State);

	/** 디버그 메시지를 출력하는 함수 (테스트 및 로그 확인 용도) */
	void PrintDebugMessage();

	void PlayBackgroundSound();

	void StartStage();
public:
	/** 현재 게임에 접속한 플레이어 컨트롤러 목록 */
	TArray<APlayerController*> PlayerControllers;

	/** 사용 가능한 요리 테이블 목록 */
	TArray<TSubclassOf<ACookingTable>> CookingTables;

	/** 스폰 매니저 객체 */
	ASpawnManager* SpawnManagerActor = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpawnManager> SpawnManagerClass = nullptr;

	/** 스테이지 매니저 객체 */
	AStageManager* StageManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "Global|Manager")
	TSubclassOf<AStageManager> StageManagerSpawner = nullptr;

private:
	/** 주문을 관리하는 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Order", meta = (AllowPrivateAccess = "true"))
	UOrderManageComponent* OrderManager = nullptr;

	/** 현재 게임 상태를 저장하는 객체 */
	ACookingGameState* CookingGameState = nullptr;

	/** 현재 게임 모드의 상태 */
	ECookingGameModeState CurState = ECookingGameModeState::ECS_None;


	/** 현재 선택된 플레이어 인덱스 */
	int CurIdx = 0;

	/** 상태 변경 체크 시간 */
	float CheckTime = 0.0f;
	bool bShowTimesUpUI = false;

	TArray<APlate*> PlateArray;

	float GameTime = 180.0f;

};
