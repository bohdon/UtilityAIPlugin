// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIAction.h"

#include "AIController.h"
#include "UtilityAI.h"
#include "UtilityAIComponent.h"


UUtilityAIAction::UUtilityAIAction()
	: ScoringMethod(EUtilityAIScoringMethod::Function),
	  Score(0.f)
{
}

UUtilityAIComponent* UUtilityAIAction::GetAIComponent() const
{
	return Cast<UUtilityAIComponent>(GetOuter());
}

AAIController* UUtilityAIAction::GetAIController() const
{
	if (UUtilityAIComponent* AIComp = GetAIComponent())
	{
		return AIComp->GetAIController();
	}
	return nullptr;
}

UWorld* UUtilityAIAction::GetWorld() const
{
	if (UUtilityAIComponent* AIComp = GetAIComponent())
	{
		return AIComp->GetWorld();
	}
	return nullptr;
}

bool UUtilityAIAction::CanCalculateScore() const
{
	return true;
}

float UUtilityAIAction::CalculateScore()
{
	switch (ScoringMethod)
	{
	case EUtilityAIScoringMethod::Data:
		return CalculateDataScore();
	case EUtilityAIScoringMethod::Function:
		return CalculateCustomScore();
	default:
		return 0.f;
	}
}

float UUtilityAIAction::CalculateDataScore()
{
	// TODO: implement
	return 0.f;
}

float UUtilityAIAction::CalculateCustomScore()
{
	unimplemented();
	return 0.f;
}

bool UUtilityAIAction::CanExecute() const
{
	return Score > SMALL_NUMBER;
}

void UUtilityAIAction::Execute()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Execute: %s"), *GetName());
	bIsExecuting = true;
	bIsAborting = false;
}

void UUtilityAIAction::Abort()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Abort: %s"), *GetName());
	bIsAborting = true;
}

void UUtilityAIAction::Tick(float DeltaTime)
{
}

void UUtilityAIAction::FinishAction()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Finished: %s"), *GetName());
	bIsExecuting = false;
	bIsAborting = false;
	OnFinished();
}

void UUtilityAIAction::OnFinished()
{
	OnFinishedEvent.Broadcast();
}
