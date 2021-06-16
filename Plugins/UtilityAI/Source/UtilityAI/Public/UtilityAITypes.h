// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "UtilityAITypes.generated.h"


/**
 * Available methods to use for performing utility AI action scoring.
 */
UENUM(BlueprintType)
enum class EUtilityAIScoringMethod : uint8
{
	// Scoring is be done via combining available blackboard data with various transformations
	Data,
	// Scoring is done by calling a custom scoring function on the action
	Function,
};
