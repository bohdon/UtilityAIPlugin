// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UtilityAIAction.h"
#include "Components/ActorComponent.h"
#include "UtilityAIComponent.generated.h"

class UUtilityAIActionSet;


/**
 * The central component that executes action scoring and selection.
 * Intended to be added to an AIController.
 */
UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUtilityAIComponent();

	UFUNCTION(BlueprintPure)
	AAIController* GetAIController() const;

	/** List of actions that are available from the start. Other actions can be added or removed at runtime. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UUtilityAIActionSet>> DefaultActionSets;

	/** Create and initialize all default action instances. */
	UFUNCTION(BlueprintCallable)
	void AddDefaultActions();

	/** Add a new action by class. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddAction(TSubclassOf<UUtilityAIAction> ActionClass);

	/** Add new actions from an action set. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddActionsFromSet(const UUtilityAIActionSet* ActionSet);

	/**
	 * Deinitialize and destroy all action instances.
	 * Usually called at the same time brain logic would stop, such as on unpossess.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DeinitializeActions();

	/** Return true if this component has an action */
	UFUNCTION(BlueprintPure)
	bool HasAction(TSubclassOf<UUtilityAIAction> ActionClass) const;

	/** Return an action by class */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UUtilityAIAction* GetAction(TSubclassOf<UUtilityAIAction> ActionClass) const;

	/** Return all utility action instances */
	void GetAllActions(TArray<UUtilityAIAction*>& OutActions) const;

protected:
	/** The instances of every action currently available */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UUtilityAIAction>> Actions;

	/** The current action being executed */
	UPROPERTY(Transient)
	TObjectPtr<UUtilityAIAction> CurrentAction;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

	/** Create a new action instance. If ScoreWeight is > 0, override the action's default score weight. */
	UUtilityAIAction* CreateActionInstance(TSubclassOf<UUtilityAIAction> ActionClass, float ScoreWeight = -1.f);

	/** Select an action to perform */
	UUtilityAIAction* SelectAction();

	/** Return true if a new action should interrupt the current action */
	virtual bool ShouldInterruptCurrentAction(UUtilityAIAction* NewAction);

	void OnCurrentActionFinished();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
