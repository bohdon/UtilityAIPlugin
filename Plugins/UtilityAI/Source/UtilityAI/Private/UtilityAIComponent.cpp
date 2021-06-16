// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIComponent.h"

#include "AIController.h"
#include "UtilityAI.h"


UUtilityAIComponent::UUtilityAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

AAIController* UUtilityAIComponent::GetAIController() const
{
	return Cast<AAIController>(GetOwner());
}

bool UUtilityAIComponent::HasAction(TSubclassOf<UUtilityAIAction> ActionClass) const
{
	for (const UUtilityAIAction* Action : Actions)
	{
		if (Action && Action->GetClass() == ActionClass)
		{
			return true;
		}
	}
	return false;
}

UUtilityAIAction* UUtilityAIComponent::GetAction(TSubclassOf<UUtilityAIAction> ActionClass) const
{
	for (UUtilityAIAction* Action : Actions)
	{
		if (Action && Action->GetClass() == ActionClass)
		{
			return Action;
		}
	}
	return nullptr;
}

void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UUtilityAIAction>& ActionClass : IntrinsicActions)
	{
		CreateActionInstance(ActionClass);
	}
}

void UUtilityAIComponent::CreateActionInstance(TSubclassOf<UUtilityAIAction> ActionClass)
{
	if (!ActionClass || HasAction(ActionClass))
	{
		// action already instanced
		return;
	}
	UUtilityAIAction* NewAction = NewObject<UUtilityAIAction>(this, ActionClass, NAME_None, RF_Transient);
	if (NewAction)
	{
		Actions.Add(NewAction);
	}
}

UUtilityAIAction* UUtilityAIComponent::SelectAction()
{
	UUtilityAIAction* BestAction = nullptr;
	float BestScore = 0.f;

	for (UUtilityAIAction* Action : Actions)
	{
		if (Action->CanCalculateScore())
		{
			Action->Score = Action->CalculateScore();
			UE_LOG(LogUtilityAI, VeryVerbose, TEXT("Score: %s: %f"), *Action->GetName(), Action->Score);

			if (Action->CanExecute() && Action->Score > BestScore)
			{
				BestAction = Action;
				BestScore = Action->Score;
			}
		}
	}

	return BestAction;
}

void UUtilityAIComponent::OnCurrentActionFinished()
{
	// clear the current action allowing it to re-execute if necessary
	if (CurrentAction)
	{
		CurrentAction->OnFinishedEvent.RemoveAll(this);
	}
	CurrentAction = nullptr;
}


void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UUtilityAIAction* BestAction = SelectAction();

	if (CurrentAction != BestAction)
	{
		if (CurrentAction && CurrentAction->IsExecuting())
		{
			CurrentAction->OnFinishedEvent.RemoveAll(this);
			CurrentAction->Abort();
			// TODO: wait for abort?
		}

		CurrentAction = BestAction;

		if (CurrentAction)
		{
			CurrentAction->OnFinishedEvent.AddUObject(this, &UUtilityAIComponent::OnCurrentActionFinished);
			CurrentAction->Execute();
		}

		// TODO: on action change event
	}

	if (CurrentAction)
	{
		CurrentAction->Tick(DeltaTime);
	}
}
