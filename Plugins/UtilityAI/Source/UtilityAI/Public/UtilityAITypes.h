// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAITypes.generated.h"


/**
 * Available methods to use for performing utility AI action scoring.
 */
UENUM(BlueprintType)
enum class EUtilityAIScoringMethod : uint8
{
	// Scoring is be done via combining available blackboard data with various transformations
	Data UMETA(DisplayName = "Data (Not Implemented)"),
	// Scoring is done by calling a custom scoring function on the action
	Function,
};


/** Types of operations used to combine score components for a Utility AI Action. */
UENUM(BlueprintType)
enum class EUtilityAIScoreOperation : uint8
{
	/** Multiply all the values together. */
	Multiply,
	/** Use the highest value. */
	Max,
	/** Use the lowest value. */
	Min,
};


/**
 * Represents a single scoring element calculated by a UtilityAIAction.
 */
USTRUCT(BlueprintType)
struct FUtilityAIScore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
};


/**
 * Represents a set of scoring elements that make up a final score.
 */
USTRUCT(BlueprintType)
struct FUtilityAIScoringElements
{
	GENERATED_BODY()

	/** The individual scores for each element. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Scores;

	/** The names of each score element. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Names;

	/** The operation to use when combining the scores. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUtilityAIScoreOperation Operation = EUtilityAIScoreOperation::Multiply;

	void AddScore(float Score, const FString& Name)
	{
		Scores.Add(Score);
		Names.Add(Name);
	}

	void AddScore(const FUtilityAIScore& ElementScore)
	{
		AddScore(ElementScore.Score, ElementScore.Name);
	}

	void Reset()
	{
		Scores.Reset();
		Names.Reset();
	}
};
