// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAIAction.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UtilityAIBehaviorStatics.generated.h"


/**
 * Static functions for working with UtilityAIBehaviorActions
 */
UCLASS(meta=(RestrictedToClasses="UtilityAIBehaviorAction"))
class UTILITYAI_API UUtilityAIBehaviorStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static UBehaviorTreeComponent* GetOwnerBehaviorComponent(UUtilityAIAction* ActionOwner);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static UBlackboardComponent* GetOwnersBlackboard(UUtilityAIAction* ActionOwner);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static UObject* GetBlackboardValueAsObject(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static AActor* GetBlackboardValueAsActor(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static UClass* GetBlackboardValueAsClass(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static uint8 GetBlackboardValueAsEnum(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static int32 GetBlackboardValueAsInt(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static float GetBlackboardValueAsFloat(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static bool GetBlackboardValueAsBool(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static FString GetBlackboardValueAsString(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static FName GetBlackboardValueAsName(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static FGameplayTag GetBlackboardValueAsTag(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static FVector GetBlackboardValueAsVector(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintPure, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static FRotator GetBlackboardValueAsRotator(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsObject(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, UObject* Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsClass(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, UClass* Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsEnum(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, uint8 Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsInt(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, int32 Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsFloat(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, float Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsBool(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, bool Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsString(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FString Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsName(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FName Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsTag(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FGameplayTag Value);

	UFUNCTION(BlueprintCallable, Meta=(HidePin="ActionOwner", DefaultToSelf="ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsVector(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FVector Value);

	UFUNCTION(BlueprintCallable, Meta = (HidePin = "ActionOwner", DefaultToSelf = "ActionOwner"), Category = "AI|UtilityAI")
	static void SetBlackboardValueAsRotator(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FRotator Value);

	/** Resets indicated value to "not set" value, based on values type */
	UFUNCTION(BlueprintCallable, Meta = (HidePin = "ActionOwner", DefaultToSelf = "ActionOwner"), Category = "AI|UtilityAI")
	static void ClearBlackboardValue(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key);
};
