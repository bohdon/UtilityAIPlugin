// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIBehaviorStatics.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Misc/RuntimeErrors.h"


UBehaviorTreeComponent* UUtilityAIBehaviorStatics::GetOwnerBehaviorComponent(UUtilityAIAction* ActionOwner)
{
	ensureAsRuntimeWarning(
		(ActionOwner != nullptr) &&
		(ActionOwner->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint) || Cast<UDynamicClass>(ActionOwner->GetClass())));

	AAIController* AIController = ActionOwner->GetAIController();
	UBehaviorTreeComponent* BehaviorComp = AIController ? Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()) : nullptr;
	ensureAsRuntimeWarning(BehaviorComp != nullptr);

	return BehaviorComp;
}

UBlackboardComponent* UUtilityAIBehaviorStatics::GetOwnersBlackboard(UUtilityAIAction* ActionOwner)
{
	UBehaviorTreeComponent* BTComponent = GetOwnerBehaviorComponent(ActionOwner);
	if (ensureAsRuntimeWarning(BTComponent != nullptr))
	{
		return BTComponent->GetBlackboardComponent();
	}
	return nullptr;
}

UObject* UUtilityAIBehaviorStatics::GetBlackboardValueAsObject(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsObject(Key.SelectedKeyName) : nullptr;
}

AActor* UUtilityAIBehaviorStatics::GetBlackboardValueAsActor(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	return Cast<AActor>(GetBlackboardValueAsObject(ActionOwner, Key));
}

UClass* UUtilityAIBehaviorStatics::GetBlackboardValueAsClass(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsClass(Key.SelectedKeyName) : nullptr;
}

uint8 UUtilityAIBehaviorStatics::GetBlackboardValueAsEnum(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsEnum(Key.SelectedKeyName) : 0;
}

int32 UUtilityAIBehaviorStatics::GetBlackboardValueAsInt(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsInt(Key.SelectedKeyName) : 0;
}

float UUtilityAIBehaviorStatics::GetBlackboardValueAsFloat(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsFloat(Key.SelectedKeyName) : 0.0f;
}

bool UUtilityAIBehaviorStatics::GetBlackboardValueAsBool(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsBool(Key.SelectedKeyName) : false;
}

FString UUtilityAIBehaviorStatics::GetBlackboardValueAsString(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsString(Key.SelectedKeyName) : FString();
}

FName UUtilityAIBehaviorStatics::GetBlackboardValueAsName(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsName(Key.SelectedKeyName) : NAME_None;
}

FGameplayTag UUtilityAIBehaviorStatics::GetBlackboardValueAsTag(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? FGameplayTag::RequestGameplayTag(BlackboardComp->GetValueAsName(Key.SelectedKeyName)) : FGameplayTag();
}

FVector UUtilityAIBehaviorStatics::GetBlackboardValueAsVector(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsVector(Key.SelectedKeyName) : FVector::ZeroVector;
}

FRotator UUtilityAIBehaviorStatics::GetBlackboardValueAsRotator(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	const UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsRotator(Key.SelectedKeyName) : FRotator::ZeroRotator;
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsObject(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, UObject* Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsClass(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, UClass* Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Class>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsEnum(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, uint8 Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Enum>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsInt(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, int32 Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Int>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsFloat(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, float Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Float>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsBool(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, bool Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Bool>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsString(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FString Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_String>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsName(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FName Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Name>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsTag(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key,
                                                        FGameplayTag Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		// TODO (bsayre): Add BlackboardKeyType_GameplayTag
		BlackboardComp->SetValue<UBlackboardKeyType_Name>(Key.SelectedKeyName, Value.GetTagName());
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsVector(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key, FVector Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::SetBlackboardValueAsRotator(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key,
                                                            FRotator Value)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Rotator>(Key.SelectedKeyName, Value);
	}
}

void UUtilityAIBehaviorStatics::ClearBlackboardValue(UUtilityAIAction* ActionOwner, const FBlackboardKeySelector& Key)
{
	if (UBlackboardComponent* BlackboardComp = GetOwnersBlackboard(ActionOwner))
	{
		BlackboardComp->ClearValue(Key.SelectedKeyName);
	}
}
