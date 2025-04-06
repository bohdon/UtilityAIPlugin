// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIComponent.h"

#include "AIController.h"
#include "UtilityAIModule.h"


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

void UUtilityAIComponent::GetAllActions(TArray<UUtilityAIAction*>& OutActions) const
{
	OutActions = Actions;
}

void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UUtilityAIAction>& ActionClass : IntrinsicActions)
	{
		CreateActionInstance(ActionClass);
	}
}

void UUtilityAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DestroyActionInstances();
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

		NewAction->Initialize();
	}
}

void UUtilityAIComponent::DestroyActionInstances()
{
	for (UUtilityAIAction* Action : Actions)
	{
		Action->Deinitialize();
		Action->ConditionalBeginDestroy();
	}

	Actions.Empty();
}

UUtilityAIAction* UUtilityAIComponent::SelectAction()
{
	UUtilityAIAction* BestAction = nullptr;

	for (UUtilityAIAction* Action : Actions)
	{
		if (Action->AreTagRequirementsMet() && Action->CanCalculateScore())
		{
			Action->Score = Action->CalculateScore();
			UE_LOG(LogUtilityAI, VeryVerbose, TEXT("Score: %s: %f"), *Action->GetName(), Action->Score);

			if (!Action->CanExecute() || Action->Score <= SMALL_NUMBER)
			{
				continue;
			}

			// either no best action, or score is better, or score is same but priority is better
			if (!BestAction || Action->Score > BestAction->Score ||
				(FMath::IsNearlyEqual(Action->Score, BestAction->Score) && Action->Priority > BestAction->Priority))
			{
				BestAction = Action;
			}
		}
	}

	return BestAction;
}

bool UUtilityAIComponent::ShouldInterruptCurrentAction(UUtilityAIAction* NewAction)
{
	return !CurrentAction || NewAction->Priority > CurrentAction->Priority;
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

	if (BestAction && CurrentAction != BestAction && ShouldInterruptCurrentAction(BestAction))
	{
		if (CurrentAction && CurrentAction->IsExecuting())
		{
			CurrentAction->OnFinishedEvent.RemoveAll(this);
			CurrentAction->StartAbort();
			// TODO: wait for abort?
		}

		CurrentAction = BestAction;

		if (CurrentAction)
		{
			CurrentAction->OnFinishedEvent.AddUObject(this, &UUtilityAIComponent::OnCurrentActionFinished);
			CurrentAction->StartExecute();
		}

		// TODO: on action change event
	}

	if (CurrentAction)
	{
		CurrentAction->Tick(DeltaTime);
	}
}
