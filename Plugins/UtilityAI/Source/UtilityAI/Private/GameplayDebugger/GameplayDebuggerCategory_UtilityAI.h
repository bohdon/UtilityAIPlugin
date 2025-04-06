// Copyright Bohdon Sayre. All Rights Reserved.

#pragma once

#if WITH_GAMEPLAY_DEBUGGER_MENU

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"


/**
 * Gameplay debugger category for Utility AI
 */
class FGameplayDebuggerCategory_UtilityAI : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_UtilityAI();

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

protected:
	struct FRepData
	{
		FString CompName;
		FString ActionsDesc;

		void Serialize(FArchive& Ar);
	};

	FRepData DataPack;
};

#endif
