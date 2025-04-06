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

	static FString FormatScoreBar(float Score, float ScoreWeight, float MaxScore);
	static FString FormatProgressBar(float Percent, int32 BarWidth, TCHAR Char = '|');

protected:
	static const FString Status_ActiveBusy;
	static const FString Status_Active;
	static const FString Status_TagsNotMet;
	static const FString Status_NoScore;
	static const FString Status_Considering;

	struct FRepData
	{
		FString CompName;
		FString ActionsDesc;

		void Serialize(FArchive& Ar);
	};

	FRepData DataPack;

	/** The height of the drawn text during the last update, for drawing a background this update. */
	float LastDrawDataHeight = 0.0f;
};

#endif
