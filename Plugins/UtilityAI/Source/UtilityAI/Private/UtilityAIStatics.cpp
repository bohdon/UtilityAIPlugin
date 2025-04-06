// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIStatics.h"

#include "UtilityAIModule.h"


float UUtilityAIStatics::CombineWeightedScores(TArray<float> Scores, TArray<float> Weights)
{
	if (Scores.Num() != Weights.Num())
	{
		UE_LOG(LogUtilityAI, Warning, TEXT("CombineWeightedScores: Weights array must have the same length as Scores"));
		return 0;
	}

	// calculate sum of all weights used to normalize the total score back to 0..1
	float TotalWeight = 0.f;
	for (const float& Weight : Weights)
	{
		TotalWeight += Weight;
	}

	float TotalScore = 0.f;
	for (int32 Idx = 0; Idx < Scores.Num(); ++Idx)
	{
		TotalScore += Scores[Idx] * Weights[Idx];
	}

	return TotalScore / TotalWeight;
}
