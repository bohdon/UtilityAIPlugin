// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIAction.h"

#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "UtilityAIModule.h"
#include "UtilityAIComponent.h"
#include "Engine/World.h"


TAutoConsoleVariable<bool> CVarDebugCalculateScores(
	TEXT("ai.Utility.DebugCalculateScores"),
	false,
	TEXT("Always calculate scores for debugging purposes, even for actions that cannot execute."));


UUtilityAIAction::UUtilityAIAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bHasBlueprintInitialize = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, Initialize_BP));
	bHasBlueprintDeinitialize = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, Deinitialize_BP));
	bHasBlueprintCalculateElementScores = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, CalculateElementScores_BP));
	bHasBlueprintCalculateScore = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, CalculateScore_BP));
	bHasBlueprintExecute = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, Execute_BP));
	bHasBlueprintAbort = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, Abort_BP));
	bHasBlueprintOnFinished = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, OnFinished_BP));
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

APawn* UUtilityAIAction::GetAIPawn() const
{
	if (const AAIController* AIController = GetAIController())
	{
		return AIController->GetPawn();
	}
	return nullptr;
}

UWorld* UUtilityAIAction::GetWorld() const
{
	if (const UUtilityAIComponent* AIComp = GetAIComponent())
	{
		return AIComp->GetWorld();
	}
	return nullptr;
}

bool UUtilityAIAction::CanCalculateScore() const
{
	return !IsScoreFrozen();
}

void UUtilityAIAction::UpdateScore()
{
	bool bShouldCalcScore = AreTagRequirementsMet() && CanCalculateScore();

#if WITH_GAMEPLAY_DEBUGGER
	// allow calculating the score all the time, but only store the scoring elements,
	// don't update the actual score when debugging
	const bool bDebugCalcScore = CVarDebugCalculateScores.GetValueOnAnyThread();
	const bool bIsDebugOnlyCalculation = !bShouldCalcScore && bDebugCalcScore;
	bShouldCalcScore |= bDebugCalcScore;
#endif

	if (!bShouldCalcScore)
	{
		return;
	}

	ScoringElements.Reset();
	const float NewScore = CalculateScore();

#if WITH_GAMEPLAY_DEBUGGER
	if (!bIsDebugOnlyCalculation)
#endif
	{
		Score = NewScore;
	}
}

float UUtilityAIAction::CalculateScore()
{
	switch (ScoringMethod)
	{
	case EUtilityAIScoringMethod::Data:
		return CalculateDataScore() * ScoreWeight;
	case EUtilityAIScoringMethod::Function:
		return CalculateCustomScore() * ScoreWeight;
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
	if (bHasBlueprintCalculateElementScores)
	{
		TArray<FUtilityAIScore> Elements;
		EUtilityAIScoreOperation Operation;
		CalculateElementScores_BP(Elements, Operation);

		// TODO: once data driven, early out when we hit a score that guarantees 0, to only calculate whats needed

		// register the scores and desired operation
		for (const FUtilityAIScore& Element : Elements)
		{
			ScoringElements.AddScore(Element);
		}
		ScoringElements.Operation = Operation;

		// calculate the result
		return CombineScores(ScoringElements.Scores, ScoringElements.Operation);
	}

	if (bHasBlueprintCalculateScore)
	{
		return CalculateScore_BP();
	}

	return 0.f;
}

float UUtilityAIAction::CombineScores(const TArray<float>& InScores, EUtilityAIScoreOperation Operation)
{
	if (InScores.IsEmpty())
	{
		return 0.f;
	}

	float Result = 0.f;
	switch (Operation)
	{
	case EUtilityAIScoreOperation::Multiply:
		Result = 1.f;
		for (const float ElementScore : InScores)
		{
			Result *= ElementScore;
		}
		break;

	case EUtilityAIScoreOperation::Max:
		for (const float ElementScore : InScores)
		{
			Result = FMath::Max(Result, ElementScore);
		}
		break;

	case EUtilityAIScoreOperation::Min:
		Result = 1.f;
		for (const float ElementScore : InScores)
		{
			Result = FMath::Min(Result, ElementScore);
		}
		break;
	}
	return Result;
}

bool UUtilityAIAction::CanExecute() const
{
	return AreTagRequirementsMet() && Score > UE_SMALL_NUMBER;
}

bool UUtilityAIAction::AreTagRequirementsMet() const
{
	if (RequireTags.IsEmpty() && IgnoreTags.IsEmpty() && TagQuery.IsEmpty())
	{
		// early out
		return true;
	}

	const IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(GetAIController());
	if (!TagInterface)
	{
		return false;
	}

	if (!TagInterface->HasAllMatchingGameplayTags(RequireTags) ||
		TagInterface->HasAnyMatchingGameplayTags(IgnoreTags))
	{
		return false;
	}

	if (!TagQuery.IsEmpty())
	{
		FGameplayTagContainer ControllerOwnedTags;
		TagInterface->GetOwnedGameplayTags(ControllerOwnedTags);
		if (!TagQuery.Matches(ControllerOwnedTags))
		{
			return false;
		}
	}

	return true;
}

void UUtilityAIAction::Initialize()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Initialize: %s"), *GetName());
	bIsInitialized = true;

	if (bHasBlueprintInitialize)
	{
		Initialize_BP();
	}
}

void UUtilityAIAction::Deinitialize()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Deinitialize: %s"), *GetName());
	bIsInitialized = false;

	if (bHasBlueprintDeinitialize)
	{
		Deinitialize_BP();
	}
}

void UUtilityAIAction::StartExecute()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Execute: %s"), *GetName());
	bIsExecuting = true;
	bIsAborting = false;

	++ExecuteCount;
	LastExecuteTime = GetWorld()->GetTimeSeconds();

	if (bHasBlueprintExecute)
	{
		Execute_BP();
	}
	else
	{
		Execute();
	}
}

void UUtilityAIAction::StartAbort()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Abort: %s"), *GetName());
	bIsAborting = true;
	if (bHasBlueprintAbort)
	{
		Abort_BP();
	}
	else
	{
		Abort();
	}
}

void UUtilityAIAction::Execute()
{
	FinishAction();
}

void UUtilityAIAction::Abort()
{
	FinishAction();
}

void UUtilityAIAction::Tick(float DeltaTime)
{
}

void UUtilityAIAction::FreezeScore()
{
	bIsScoreFrozen = true;
}

void UUtilityAIAction::UnfreezeScore()
{
	bIsScoreFrozen = false;
}

void UUtilityAIAction::FinishAction()
{
	UE_LOG(LogUtilityAI, Verbose, TEXT("Finished: %s"), *GetName());
	bIsExecuting = false;
	bIsAborting = false;

	if (const UWorld* World = GetWorld())
	{
		LastFinishTime = GetWorld()->GetTimeSeconds();
	}

	OnFinished();
}

void UUtilityAIAction::OnFinished()
{
	OnFinishedEvent.Broadcast();

	if (bHasBlueprintOnFinished)
	{
		OnFinished_BP();
	}
}
