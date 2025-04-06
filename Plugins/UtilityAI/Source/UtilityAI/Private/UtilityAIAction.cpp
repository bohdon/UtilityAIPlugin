// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIAction.h"

#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "UtilityAIModule.h"
#include "UtilityAIComponent.h"


UUtilityAIAction::UUtilityAIAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  ScoringMethod(EUtilityAIScoringMethod::Function),
	  ScoreWeight(1.f),
	  Score(0.f)
{
	bHasBlueprintInitialize = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, Initialize_BP));
	bHasBlueprintDeinitialize = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUtilityAIAction, Deinitialize_BP));
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
	return true;
}

float UUtilityAIAction::CalculateScore()
{
	float CalculatedScore = 0.f;
	switch (ScoringMethod)
	{
	case EUtilityAIScoringMethod::Data:
		CalculatedScore = CalculateDataScore();
		break;
	case EUtilityAIScoringMethod::Function:
		CalculatedScore = CalculateCustomScore();
		break;
	default:
		break;
	}

	return CalculatedScore * ScoreWeight;
}

float UUtilityAIAction::CalculateDataScore()
{
	// TODO: implement
	return 0.f;
}

float UUtilityAIAction::CalculateCustomScore()
{
	if (bHasBlueprintCalculateScore)
	{
		return CalculateScore_BP();
	}
	return 0.f;
}

bool UUtilityAIAction::CanExecute() const
{
	return Score > SMALL_NUMBER;
}

bool UUtilityAIAction::AreTagRequirementsMet() const
{
	if (RequireTags.IsEmpty() && IgnoreTags.IsEmpty() && TagQuery.IsEmpty())
	{
		// early out
		return true;
	}

	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(GetAIController());
	if (!TagInterface)
	{
		return false;
	}

	if (!TagInterface->HasAllMatchingGameplayTags(RequireTags))
	{
		return false;
	}

	if (TagInterface->HasAnyMatchingGameplayTags(IgnoreTags))
	{
		return false;
	}

	if (TagInterface->HasAnyMatchingGameplayTags(CooldownTags))
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

const FGameplayTagContainer* UUtilityAIAction::GetCooldownTags() const
{
	return &CooldownTags;
}

void UUtilityAIAction::CommitCooldown()
{
	// TODO: implement
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

	if (bHasBlueprintOnFinished)
	{
		OnFinished_BP();
	}
}
