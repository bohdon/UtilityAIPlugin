// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FinishUtilityAction.generated.h"


/**
 * Finish an active utility action.
 */
UCLASS()
class UTILITYAI_API UBTTask_FinishUtilityAction : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FinishUtilityAction(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
