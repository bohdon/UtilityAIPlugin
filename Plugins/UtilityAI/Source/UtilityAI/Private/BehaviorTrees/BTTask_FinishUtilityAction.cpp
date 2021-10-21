// Copyright Bohdon Sayre. All Rights Reserved.


#include "BehaviorTrees/BTTask_FinishUtilityAction.h"

#include "UtilityAIAction.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTask_FinishUtilityAction::UBTTask_FinishUtilityAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Finish Utility Action";
	
	// accept only UtilityAIAction objects
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FinishUtilityAction, BlackboardKey),
	                              UUtilityAIAction::StaticClass());
}

EBTNodeResult::Type UBTTask_FinishUtilityAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{
		UObject* KeyValue = BBComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		UUtilityAIAction* UtilityAction = Cast<UUtilityAIAction>(KeyValue);
		if (UtilityAction && UtilityAction->IsExecuting())
		{
			UtilityAction->FinishAction();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

FString UBTTask_FinishUtilityAction::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s %s"), *Super::GetStaticDescription(), *BlackboardKey.SelectedKeyName.ToString());
}
