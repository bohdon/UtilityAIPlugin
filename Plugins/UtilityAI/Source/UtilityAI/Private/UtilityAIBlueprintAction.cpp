// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIBlueprintAction.h"


float UUtilityAIBlueprintAction::CalculateCustomScore()
{
	return CalculateCustomScore_BP();
}

void UUtilityAIBlueprintAction::Execute()
{
	Super::Execute();

	Execute_BP();
}

void UUtilityAIBlueprintAction::Abort()
{
	Super::Abort();

	Abort_BP();
}
