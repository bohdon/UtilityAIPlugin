// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIActionSet.h"

#include "UtilityAIAction.h"


void UUtilityAIActionSet::SortByWeight()
{
	// sort by name first, for stability when scores match
	Actions.KeySort([&](const TSubclassOf<UUtilityAIAction>& A, const TSubclassOf<UUtilityAIAction>& B)
	{
		if (!A || !B)
		{
			return !!A;
		}
		return A->GetName().Compare(B->GetName()) < 0;
	});

	Actions.ValueSort([&](const float A, const float B)
	{
		return A > B;
	});
}

#if WITH_EDITOR
void UUtilityAIActionSet::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UUtilityAIActionSet, Actions))
	{
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
		{
			// default to 1.0
			const int32 AddedAtIndex = PropertyChangedEvent.GetArrayIndex(PropertyChangedEvent.Property->GetFName().ToString());
			int32 Idx = 0;
			for (TTuple<TSubclassOf<UUtilityAIAction>, float>& Elem : Actions)
			{
				if (Idx == AddedAtIndex)
				{
					Elem.Value = 1.f;
					break;
				}
				++Idx;
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
