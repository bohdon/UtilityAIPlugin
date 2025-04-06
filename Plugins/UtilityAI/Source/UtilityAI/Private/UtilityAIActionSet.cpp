// Copyright Bohdon Sayre. All Rights Reserved.


#include "UtilityAIActionSet.h"

#include "UtilityAIAction.h"


void UUtilityAIActionSet::SortByWeight()
{
	Actions.KeySort([&](const TSubclassOf<UUtilityAIAction>& A, const TSubclassOf<UUtilityAIAction>& B)
	{
		if (!A || !B)
		{
			return !!A;
		}
		const float MaxScoreA = Actions.FindChecked(A);
		const float MaxScoreB = Actions.FindChecked(B);
		if (MaxScoreA != MaxScoreB)
		{
			return MaxScoreA > MaxScoreB;
		}
		return A->GetName().Compare(B->GetName()) < 0;
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
