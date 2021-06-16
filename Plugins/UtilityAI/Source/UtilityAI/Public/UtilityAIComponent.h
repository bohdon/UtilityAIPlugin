// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UtilityAIAction.h"
#include "Components/ActorComponent.h"
#include "UtilityAIComponent.generated.h"


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

	/**
	 * List of actions that are available from the start.
	 * Other actions can be added or removed at runtime.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowAbstract = "false"))
	TArray<TSubclassOf<UUtilityAIAction>> IntrinsicActions;

	/** Return true if this component has an action */
	UFUNCTION(BlueprintPure)
	bool HasAction(TSubclassOf<UUtilityAIAction> ActionClass) const;

	/** Return an action by class */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UUtilityAIAction* GetAction(TSubclassOf<UUtilityAIAction> ActionClass) const;

protected:
	/** The instances of every action currently available */
	UPROPERTY(Transient)
	TArray<UUtilityAIAction*> Actions;

	/** The current action being executed */
	UPROPERTY(Transient)
	UUtilityAIAction* CurrentAction;

	virtual void BeginPlay() override;

	void CreateActionInstance(TSubclassOf<UUtilityAIAction> ActionClass);

	/** Select an action to perform */
	UUtilityAIAction* SelectAction();

	void OnCurrentActionFinished();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
