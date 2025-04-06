// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIBehaviorAction.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UUtilityAIBehaviorAction::UUtilityAIBehaviorAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UtilityActionKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UUtilityAIBehaviorAction, UtilityActionKey),
	                                 UUtilityAIAction::StaticClass());
}

UBlackboardData* UUtilityAIBehaviorAction::GetBlackboardAsset() const
{
	return DefaultBehaviorAsset ? DefaultBehaviorAsset->GetBlackboardAsset() : nullptr;
}

void UUtilityAIBehaviorAction::InitBlackboardKeys()
{
	if (const UBlackboardData* BBData = GetBlackboardAsset())
	{
		UtilityActionKey.ResolveSelectedKey(*BBData);
	}
	else
	{
		UtilityActionKey.InvalidateResolvedKey();
	}
}

bool UUtilityAIBehaviorAction::RunBehaviorTree(UBehaviorTree* BehaviorTree, bool bSingleRun)
{
	// TODO (bsayre): Weird having so much authority over the AI controller here, maybe move to cleaner statics?

	AAIController* AIController = GetAIController();
	if (!BehaviorTree || !AIController)
	{
		return false;
	}

	bool bSuccess = true;

	// get or create blackboard, don't change if existing blackboard is compatible with bt asset
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (BehaviorTree->BlackboardAsset &&
		(BlackboardComp == nullptr || !BlackboardComp->IsCompatibleWith(BehaviorTree->BlackboardAsset)))
	{
		bSuccess = AIController->UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp);
	}

	if (bSuccess)
	{
		// set initial blackboard values
		if (BlackboardComp)
		{
			if (BehaviorTree->BlackboardAsset)
			{
				UtilityActionKey.ResolveSelectedKey(*BehaviorTree->BlackboardAsset);
			}

			// store a reference to this utility action
			BlackboardComp->SetValue<UBlackboardKeyType_Object>(UtilityActionKey.GetSelectedKeyID(), this);
		}

		BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
		if (!BTComp.IsValid())
		{
			BTComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BTComponent"));
			BTComp->RegisterComponent();
		}
		check(BTComp.IsValid());

		// assign newly created brain component
		AIController->BrainComponent = BTComp.Get();

		BTComp->StartTree(*BehaviorTree, bSingleRun ? EBTExecutionMode::SingleRun : EBTExecutionMode::Looped);
		CurrentBehavior = BehaviorTree;
		bIsBehaviorRunning = true;
	}

	return bSuccess;
}

bool UUtilityAIBehaviorAction::StopBehaviorTree(UBehaviorTree* BehaviorTree)
{
	if (const AAIController* AIController = GetAIController())
	{
		if (UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
		{
			if (BehaviorComp->GetRootTree() == BehaviorTree)
			{
				BehaviorComp->StopTree(EBTStopMode::Safe);
				// intentionally clear running flag, but leave CurrentBehavior in case someone wants it
				bIsBehaviorRunning = false;
				return true;
			}
		}
	}
	return false;
}

bool UUtilityAIBehaviorAction::RunDefaultBehaviorTree()
{
	return RunBehaviorTree(DefaultBehaviorAsset, bSingleRunBehavior);
}

void UUtilityAIBehaviorAction::OnBehaviorTreeFinished_Implementation()
{
	if (bFinishWhenBehaviorStops)
	{
		FinishAction();
	}
}

void UUtilityAIBehaviorAction::Initialize()
{
	Super::Initialize();

	InitBlackboardKeys();
}

void UUtilityAIBehaviorAction::Execute()
{
	RunDefaultBehaviorTree();
}

void UUtilityAIBehaviorAction::Abort()
{
	StopBehaviorTree(DefaultBehaviorAsset);
	FinishAction();
}

void UUtilityAIBehaviorAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// poll to see if the behavior has stopped
	// TODO (bsayre): add events to behavior tree component that we can bind to instead of polling
	if (bIsBehaviorRunning && CurrentBehavior.IsValid() && BTComp.IsValid())
	{
		if (!BTComp->IsRunning() || BTComp->GetRootTree() != CurrentBehavior.Get())
		{
			// behavior is finished, notify the action
			// intentionally clear running flag, but leave CurrentBehavior in case someone wants it
			bIsBehaviorRunning = false;
			OnBehaviorTreeFinished();
		}
	}
}

#if WITH_EDITOR
void UUtilityAIBehaviorAction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UUtilityAIBehaviorAction, DefaultBehaviorAsset))
	{
		InitBlackboardKeys();
	}
}
#endif
