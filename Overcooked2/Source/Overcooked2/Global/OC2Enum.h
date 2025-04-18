// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OC2Enum.generated.h"

UENUM(BlueprintType)
enum class ECookingGameModeState : uint8
{
	ECS_None	UMETA(DisplayName = "None"),
	ECS_Stay    UMETA(DisplayName = "Stay"),
	ECS_Stage   UMETA(DisplayName = "Stage"),
	ECS_TimeUp   UMETA(DisplayName = "TimeUp"),
	ECS_Score   UMETA(DisplayName = "Score"),
};

UENUM(BlueprintType)
enum class ECookingStageState : uint8
{
	ESS_NONE				UMETA(DisplayName = "None"),
	ESS_WAITING_TO_START	UMETA(DisplayName = "대기 중"),
	ESS_IN_PROGRESS			UMETA(DisplayName = "진행 중"),
	ESS_WAITING_POST_MATCH	UMETA(DisplayName = "대기 중 (포스트 매치)"),
};

UENUM(BlueprintType)
enum class EPotState : uint8
{
	IDLE 			UMETA(DisplayName = "대기"),
	HEATING			UMETA(DisplayName = "HEATING"),
	BOILING			UMETA(DisplayName = "BOILING"),
	COOKED			UMETA(DisplayName = "COOKED"),
	COOKED_WARNING	UMETA(DisplayName = "COOKED_WARNING"),
	COOKED_DANGER	UMETA(DisplayName = "COOKED_DANGER"),
	SCORCHING	 	UMETA(DisplayName = "SCORCHING"),
	OVERCOOKED		UMETA(DisplayName = "OVERCOOKED"),
	MAX				UMETA(DisplayName = "MAX"),
};

UENUM(BlueprintType)
enum class EPlateState : uint8
{
	NONE			UMETA(DisplayName = "None"),
	EMPTY			UMETA(DisplayName = "빈 접시"),
	OCCUPIED		UMETA(DisplayName = "요리 중"),
	COMPLETED		UMETA(DisplayName = "요리 완성"),
	DIRTY			UMETA(DisplayName = "세척 전 접시"),
};

UENUM(BlueprintType)
enum class EOC2Stage : uint8
{
	EOS_None		UMETA(DisplayName = "None"),
	EOS_Sushi_1_1	UMETA(DisplayName = "스시레벨 1 - 1"),
	EOS_Sushi_1_2	UMETA(DisplayName = "스시레벨 1 - 2"),
	EOS_Sushi_1_3	UMETA(DisplayName = "스시레벨 1 - 3"),
	EOS_Sushi_1_4	UMETA(DisplayName = "스시레벨 1 - 4"),
	EOS_Sushi_1_5	UMETA(DisplayName = "스시레벨 1 - 5"),
};

UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	EIT_NONE			UMETA(DisplayName = "None"),
	EIT_FISH			UMETA(DisplayName = "생선"),
	//EIT_CABBAGE			UMETA(DisplayName = "양배추"),
	EIT_PRAWN			UMETA(DisplayName = "새우"),
	//EIT_TOMATO			UMETA(DisplayName = "토마토"),
	EIT_SEAWEED			UMETA(DisplayName = "김"),
	EIT_RICE			UMETA(DisplayName = "쌀"),
	EIT_CUCUMBER		UMETA(DisplayName = "오이"),
};

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	EIS_NONE      		UMETA(DisplayName = "None"),
	EIS_CHOPPABLE		UMETA(DisplayName = "손질 전"),
	EIS_CHOPPED   		UMETA(DisplayName = "손질 후"),
	EIS_GRILLABLE    	UMETA(DisplayName = "굽기 전"),
	EIS_GRILLED      	UMETA(DisplayName = "구운 후"),
	EIS_FRYABLE     	UMETA(DisplayName = "튀기기 전"),
	EIS_FRYABED     	UMETA(DisplayName = "튀기기 후"),
	EIS_BOILABLE     	UMETA(DisplayName = "끓이기 전"),
	EIS_BOILED      	UMETA(DisplayName = "끓이기 후"),
	EIS_FINISHED     	UMETA(DisplayName = "완제품"),
	EIS_OVERCOOKED      UMETA(DisplayName = "태움"),
	EIS_MAX				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class ECookingType : uint8
{
	ECT_NONE				UMETA(DisplayName = "None"),
	ECT_INGREDIENT			UMETA(DisplayName = "재료"),
	ECT_PLATE				UMETA(DisplayName = "요리"),
	ECT_POT					UMETA(DisplayName = "냄비"),
	ECT_FIREEXTINGUISHER	UMETA(DisplayName = "소화기"),
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2Enum : public UObject
{
	GENERATED_BODY()
	
};
