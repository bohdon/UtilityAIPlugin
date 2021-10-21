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

	/**
	 * The priority of this action.
	 * Higher priorities will abort lower priorities when an action is selected.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 Priority;

	/** The scoring method to use for this action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EUtilityAIScoringMethod ScoringMethod;

	/** A multiplier applied to the calculated score of this action affecting it's weight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float ScoreWeight;

	/** The AIController must have all of these tags for this action to be executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer RequireTags;

	/** The AIController must have none of these tags for this action to be executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer IgnoreTags;

	/** The AIController must match this tag query for this action to be executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagQuery TagQuery;

	/** The duration of the cooldown to apply when this action is executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
	float Cooldown;

	/** The cooldown tags to use for this action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	/** The last known utility score calculated for this action */
	UPROPERTY(Transient)
	float Score;

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

	/** Calculate the score for this action given the current context */
	float CalculateScore();

	/** Calculate the score of this action using data driven parameters */
	float CalculateDataScore();

	/** Perform a custom calculation to determine the current score of this action */
	virtual float CalculateCustomScore();

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

	/** Return the cooldown tags to use for this action. */
	virtual const FGameplayTagContainer* GetCooldownTags() const;

	/** Commit the cooldown for this action. */
	UFUNCTION(BlueprintCallable, Category = "AI|UtilityAI")
	virtual void CommitCooldown();

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
