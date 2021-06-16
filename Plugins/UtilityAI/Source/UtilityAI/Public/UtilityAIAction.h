// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "UtilityAITypes.h"
#include "UObject/Object.h"
#include "UtilityAIAction.generated.h"

class AAIController;
class UUtilityAIComponent;


/**
 * An action that can be scored and activated within a utility AI system.
 */
UCLASS(Abstract, BlueprintType)
class UTILITYAI_API UUtilityAIAction : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIAction();

	/** Tags that this action owns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer OwnedTags;

	/**
	 * The scoring method to use for this action.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUtilityAIScoringMethod ScoringMethod;

	/** The last known utility score calculated for this action */
	UPROPERTY(Transient)
	float Score;

	/** Return the UtilityAIComponent that owns this action */
	UFUNCTION(BlueprintPure)
	UUtilityAIComponent* GetAIComponent() const;

	/** Return the AIController that owns this action */
	UFUNCTION(BlueprintPure)
	AAIController* GetAIController() const;

	virtual UWorld* GetWorld() const override;

	/** Return true if this action is currently allowed to calculate its score */
	virtual bool CanCalculateScore() const;

	/** Calculate the score for this action given the current context */
	float CalculateScore();

	/** Calculate the score of this action using data driven parameters */
	float CalculateDataScore();

	/** Perform a custom calculation to determine the current score of this action */
	virtual float CalculateCustomScore();

	/** Return true if this action is currently allowed to be executed */
	virtual bool CanExecute() const;

	UFUNCTION(BlueprintPure)
	bool IsExecuting() const { return bIsExecuting; }

	UFUNCTION(BlueprintPure)
	bool IsAborting() const { return bIsAborting; }

	/** Execute the action. Must call FinishAction eventually */
	virtual void Execute();

	/** Abort the action if it is currently active. Must call FinishAction eventually */
	virtual void Abort();

	/** Tick the action while it is executing */
	virtual void Tick(float DeltaTime);

	/** Execute the action. Must call FinishAction eventually */
	UFUNCTION(BlueprintCallable)
	void FinishAction();

	/** Called when the action has finished */
	virtual void OnFinished();

	DECLARE_MULTICAST_DELEGATE(FOnFinishedDelegate);

	/** Called when the action has finished executing */
	FOnFinishedDelegate OnFinishedEvent;

protected:
	/** Is the action currently being executed? */
	UPROPERTY(Transient)
	bool bIsExecuting;

	/** Is the action currently being aborted? */
	UPROPERTY(Transient)
	bool bIsAborting;
};
