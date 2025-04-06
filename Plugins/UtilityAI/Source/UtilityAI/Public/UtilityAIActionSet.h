// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UtilityAIActionSet.generated.h"

class UUtilityAIAction;


/**
 * A collection of actions and their relative score weighting.
 */
UCLASS()
class UTILITYAI_API UUtilityAIActionSet : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Map of actions to add and their score weight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ForceInlineRow, UIMin = "0.01", UIMax = "100", AllowAbstract = "false"), Category = "Actions")
	TMap<TSubclassOf<UUtilityAIAction>, float> Actions;

	/** Sort the actions by score weight. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Actions")
	void SortByWeight();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
