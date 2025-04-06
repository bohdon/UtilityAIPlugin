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
UCLASS(Abstract, BlueprintType, Blueprintable)
class UTILITYAI_API UUtilityAIAction : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIAction(const FObjectInitializer& ObjectInitializer);

	/** Tags that this action has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer OwnedTags;

	/** Interrupt actions that have any of these tags, even from a busy state. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer InterruptActionsWithTags;

	/** A multiplier applied the calculated score, effectively determining the max score for this action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float ScoreWeight = 1.f;

	/**
	 * Should the score be frozen when the action is active? Useful to prevent selection hysteresis.
	 * Disable for long actions that should keep re-evaluating themselves.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bFreezeScoreWhenActive = true;

	/** The scoring method to use for this action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EUtilityAIScoringMethod ScoringMethod = EUtilityAIScoringMethod::Function;

	/** The AIController must have all of these tags for this action to be executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer RequireTags;

	/** The AIController must have none of these tags for this action to be executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer IgnoreTags;

	/** The AIController must match this tag query for this action to be executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagQuery TagQuery;

protected:
	/** The current score for this action. */
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Score = 0.f;

	/** Is the score locked at its current value? Useful to prevent scores from dropping while active. */
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsScoreFrozen = false;

	/** Detailed information about the last known score calculated for this action. */
	UPROPERTY(Transient, BlueprintReadOnly)
	FUtilityAIScoringElements ScoringElements;

public:
	/** Return the current score for this action. */
	FORCEINLINE float GetScore() const { return Score; }

	/** Return the current score elements for this action. */
	FORCEINLINE const FUtilityAIScoringElements& GetScoringElements() const { return ScoringElements; }

	/** The number of times this action has been executed. */
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 ExecuteCount = 0;

	/** If > 0, the time at which this action was last executed. */
	UPROPERTY(Transient, BlueprintReadOnly)
	float LastExecuteTime = -1000;

	/** If > 0, the time at which this action was last finished. */
	UPROPERTY(Transient, BlueprintReadOnly)
	float LastFinishTime = -1000;

	/** Return the UtilityAIComponent that owns this action */
	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	UUtilityAIComponent* GetAIComponent() const;

	/** Return the AIController that owns this action */
	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	AAIController* GetAIController() const;

	/** Return the Pawn controlled by the owning AIController */
	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	APawn* GetAIPawn() const;

	virtual UWorld* GetWorld() const override;

	/** Return true if this action is currently allowed to calculate its score */
	virtual bool CanCalculateScore() const;

	/**
	 * Calculate and store the score for this action.
	 * Access the score afterward with `GetScore`.
	 */
	void UpdateScore();

	/** Calculate the score for this action given the current context */
	float CalculateScore();

	/** Calculate the score of this action using data driven parameters */
	float CalculateDataScore();

	/** Perform a custom calculation to determine the current score of this action */
	virtual float CalculateCustomScore();

	/** Calculate a score from a set of scores. */
	virtual float CombineScores(const TArray<float>& InScores, EUtilityAIScoreOperation Operation);

	/** Return true if this action is currently allowed to be executed */
	virtual bool CanExecute() const;

	/** Return true if the AIController matches this action's tag requirements. */
	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	virtual bool AreTagRequirementsMet() const;

	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	bool IsExecuting() const { return bIsExecuting; }

	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	bool IsAborting() const { return bIsAborting; }

	/** Initialize the action. Called once when the action instance is created. */
	virtual void Initialize();

	/** Deinitialize the action. Called once when the action instance is destroyed. */
	virtual void Deinitialize();

	/** Wrapper that changes state and calls either blueprint or native execute */
	void StartExecute();

	/** Wrapper that changes state and calls either blueprint or native abort */
	void StartAbort();

	/** Execute the action. Must call FinishAction eventually */
	virtual void Execute();

	/** Abort the action if it is currently active. Must call FinishAction eventually */
	virtual void Abort();

	/** Tick the action while it is executing */
	virtual void Tick(float DeltaTime);

	/**
	 * Return true if the action is busy and should not be interrupted, even by higher-scoring actions.
	 * Can be interrupted by InterruptActionsWithTags.
	 */
	virtual bool IsBusy() const { return false; }

	UFUNCTION(BlueprintPure, Category = "AI|UtilityAI")
	virtual bool IsScoreFrozen() const { return bIsScoreFrozen || (bFreezeScoreWhenActive && IsExecuting()); }

	/** Freeze the current score so it doesn't update. */
	UFUNCTION(BlueprintCallable, Category = "AI|UtilityAI")
	void FreezeScore();

	/** Unfreeze the current score, allowing it to update. */
	UFUNCTION(BlueprintCallable, Category = "AI|UtilityAI")
	void UnfreezeScore();

	/** Finish the action. */
	UFUNCTION(BlueprintCallable, Category = "AI|UtilityAI")
	void FinishAction();

	/** Called when the action has finished */
	virtual void OnFinished();

	DECLARE_MULTICAST_DELEGATE(FOnFinishedDelegate);

	/** Called when the action has finished executing */
	FOnFinishedDelegate OnFinishedEvent;

	/** Initialize the action. Called once when the action is created. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Initialize", ScriptName = "Initialize"))
	void Initialize_BP();

	/** Deinitialize the action. Called once when the action is destroyed. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Deinitialize", ScriptName = "Deinitialize"))
	void Deinitialize_BP();

	/** Calculate scoring elements to be combined using the specified operation. */
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "CalculateElementScores", ScriptName = "CalculateElementScores"))
	void CalculateElementScores_BP(TArray<FUtilityAIScore>& Scores, EUtilityAIScoreOperation& Operation);

	/** Perform a custom calculation to determine the current score of this action */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CalculateScore", ScriptName = "CalculateScore"))
	float CalculateScore_BP();

	/** Execute the action. Must call FinishAction eventually */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Execute", ScriptName = "Execute"))
	void Execute_BP();

	/** Abort the action. Must call FinishAction eventually */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Abort", ScriptName = "Abort"))
	void Abort_BP();

	/** Called when the action has finished */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFinished", ScriptName = "OnFinished"))
	void OnFinished_BP();

protected:
	bool bHasBlueprintInitialize;
	bool bHasBlueprintDeinitialize;
	bool bHasBlueprintCalculateElementScores;
	bool bHasBlueprintCalculateScore;
	bool bHasBlueprintExecute;
	bool bHasBlueprintAbort;
	bool bHasBlueprintOnFinished;

	/** Is the action initialized? */
	UPROPERTY(Transient)
	bool bIsInitialized;

	/** Is the action currently being executed? */
	UPROPERTY(Transient)
	bool bIsExecuting;

	/** Is the action currently being aborted? */
	UPROPERTY(Transient)
	bool bIsAborting;
};
