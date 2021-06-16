// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UtilityAIAction.h"

#include "UtilityAIBlueprintAction.generated.h"


/**
 * Base class for a UtilityAIAction to be implemented in Blueprint
 */
UCLASS(Abstract, Blueprintable)
class UTILITYAI_API UUtilityAIBlueprintAction : public UUtilityAIAction
{
	GENERATED_BODY()

public:
	virtual float CalculateCustomScore() override;

	/** Perform a custom calculation to determine the current score of this action */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CalculateCustomScore"))
	float CalculateCustomScore_BP();

	virtual void Execute() override;

	/** Execute the action. Must call FinishAction eventually */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Execute"))
	void Execute_BP();

	virtual void Abort() override;

	/** Abort the action. Must call FinishAction eventually */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Abort"))
	void Abort_BP();
};
