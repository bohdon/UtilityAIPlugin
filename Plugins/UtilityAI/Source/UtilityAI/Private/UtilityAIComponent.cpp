// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIComponent.h"

#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "UtilityAIActionSet.h"
#include "UtilityAIModule.h"


UUtilityAIComponent::UUtilityAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.025f;

	// this component should be activated after possessing a valid pawn,
	// so actions can initialize with full context
	bAutoActivate = false;
}

AAIController* UUtilityAIComponent::GetAIController() const
{
	return Cast<AAIController>(GetOwner());
}

void UUtilityAIComponent::AddDefaultActions()
{
	for (const UUtilityAIActionSet* ActionSet : DefaultActionSets)
	{
		AddActionsFromSet(ActionSet);
	}
}

void UUtilityAIComponent::AddAction_Implementation(TSubclassOf<UUtilityAIAction> ActionClass)
{
	CreateActionInstance(ActionClass);
}

void UUtilityAIComponent::AddActionsFromSet_Implementation(const UUtilityAIActionSet* ActionSet)
{
	for (const auto& Elem : ActionSet->Actions)
	{
		CreateActionInstance(Elem.Key, Elem.Value);
	}
}

void UUtilityAIComponent::DeinitializeActions()
{
	for (UUtilityAIAction* Action : Actions)
	{
		Action->Deinitialize();
		Action->ConditionalBeginDestroy();
	}
	Actions.Empty();
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

bool UUtilityAIComponent::IsBusy() const
{
	if (const IGameplayTagAssetInterface* OwnerTagInterface = Cast<IGameplayTagAssetInterface>(GetOwner()))
	{
		if (OwnerTagInterface->HasAnyMatchingGameplayTags(BusyTags))
		{
			return true;
		}
	}
	if (CurrentAction && CurrentAction->IsBusy())
	{
		return true;
	}
	return false;
}

void UUtilityAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeActions();

	Super::EndPlay(EndPlayReason);
}

void UUtilityAIComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	AddDefaultActions();
}

void UUtilityAIComponent::Deactivate()
{
	DeinitializeActions();

	Super::Deactivate();
}

UUtilityAIAction* UUtilityAIComponent::CreateActionInstance(TSubclassOf<UUtilityAIAction> ActionClass, float ScoreWeight)
{
	if (!ActionClass || HasAction(ActionClass))
	{
		// action already instanced
		return nullptr;
	}

	UUtilityAIAction* NewAction = NewObject<UUtilityAIAction>(this, ActionClass, NAME_None, RF_Transient);
	if (NewAction)
	{
		if (ScoreWeight >= 0.f)
		{
			NewAction->ScoreWeight = ScoreWeight;
		}

		Actions.Add(NewAction);

		NewAction->Initialize();
	}
	return NewAction;
}

UUtilityAIAction* UUtilityAIComponent::SelectAction()
{
	UUtilityAIAction* BestAction = nullptr;

	for (UUtilityAIAction* Action : Actions)
	{
		Action->UpdateScore();

		if (!Action->CanExecute())
		{
			continue;
		}

		if (!BestAction)
		{
			BestAction = Action;
			continue;
		}

		const float ScoreToBeat = BestAction->GetScore() + (BestAction->IsExecuting() ? ScoreHysteresisThreshold : 0.f);
		if (Action->GetScore() > ScoreToBeat)
		{
			BestAction = Action;
		}
	}

	return BestAction;
}

bool UUtilityAIComponent::CanActivateAction(UUtilityAIAction* NewAction)
{
	// when busy, don't allow starting a new action, even if no action is active
	return !IsBusy() || (CurrentAction && CurrentAction->OwnedTags.HasAny(NewAction->InterruptActionsWithTags));
}

void UUtilityAIComponent::AbortCurrentAction()
{
	if (CurrentAction && CurrentAction->IsExecuting())
	{
		CurrentAction->OnFinishedEvent.RemoveAll(this);
		CurrentAction->StartAbort();
	}
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


void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsActive())
	{
		return;
	}

	UUtilityAIAction* BestAction = SelectAction();

	if (BestAction && BestAction != CurrentAction && CanActivateAction(BestAction))
	{
		AbortCurrentAction();

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
