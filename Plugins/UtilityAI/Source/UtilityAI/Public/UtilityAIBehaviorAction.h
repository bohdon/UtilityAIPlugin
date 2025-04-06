// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAIAction.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardAssetProvider.h"
#include "UtilityAIBehaviorAction.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardData;


/**
 * A Utility AI action that runs a behavior tree when executed. 
 */
UCLASS(Abstract)
class UTILITYAI_API UUtilityAIBehaviorAction : public UUtilityAIAction,
                                               public IBlackboardAssetProvider
{
	GENERATED_BODY()


public:
	UUtilityAIBehaviorAction(const FObjectInitializer& ObjectInitializer);

	/** The behavior to run when this action is executed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> DefaultBehaviorAsset;

	/** If true, don't loop the behavior when run, let it execute once then finish. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSingleRunBehavior = false;

	/** If true, finish this action when the behavior finishes running, requires bSingleRun to be true. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFinishWhenBehaviorStops = false;

	/** The blackboard key in which to store a reference to this utility action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector UtilityActionKey;

	/** IBlackboardAssetProvider interface */
	virtual UBlackboardData* GetBlackboardAsset() const override;

	/** Initialize blackboard keys for a behavior tree asset */
	virtual void InitBlackboardKeys();

	/** Run the action's behavior tree on the owning AI Controller */
	UFUNCTION(BlueprintCallable)
	bool RunBehaviorTree(UBehaviorTree* BehaviorTree, bool bSingleRun = false);

	/** Stop the action's behavior tree on the owning ai controller, if it's currently active */
	UFUNCTION(BlueprintCallable)
	bool StopBehaviorTree(UBehaviorTree* BehaviorTree);

	/** Run the action's default behavior tree */
	UFUNCTION(BlueprintCallable)
	bool RunDefaultBehaviorTree();

	/**
	 * Called when the behavior tree started by this action has finished.
	 * Only called when using SingleRun = True with RunBehaviorTree.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviorTreeFinished();

	virtual void Initialize() override;
	virtual void Execute() override;
	virtual void Abort() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** Weak reference to the behavior tree component, set after running a behavior. */
	TWeakObjectPtr<UBehaviorTreeComponent> BTComp;

	/** The current or last behavior being run by this action */
	TWeakObjectPtr<UBehaviorTree> CurrentBehavior;

	bool bIsBehaviorRunning = false;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
