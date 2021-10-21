// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAIStatics.generated.h"


/**
 * Statics for working with Utility AI actions and behaviors
 */
UCLASS()
class UTILITYAI_API UUtilityAIStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Combine an array of 0..1 scores with the given weights.
	 * @return The 0..1 combined score with all weights applied.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|UtilityAI")
	static float CombineWeightedScores(TArray<float> Scores, TArray<float> Weights);
};
