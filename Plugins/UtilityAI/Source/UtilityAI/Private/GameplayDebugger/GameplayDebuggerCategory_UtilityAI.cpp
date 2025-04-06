// Copyright Bohdon Sayre. All Rights Reserved.


#include "GameplayDebuggerCategory_UtilityAI.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "UtilityAIComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


FGameplayDebuggerCategory_UtilityAI::FGameplayDebuggerCategory_UtilityAI()
{
	SetDataPackReplication<FRepData>(&DataPack);
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_UtilityAI::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_UtilityAI());
}

void FGameplayDebuggerCategory_UtilityAI::FRepData::Serialize(FArchive& Ar)
{
	Ar << CompName;
	Ar << ActionsDesc;
}

void FGameplayDebuggerCategory_UtilityAI::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (DebugActor)
	{
		const APawn* DebugPawn = Cast<APawn>(DebugActor);
		const AController* DebugController = DebugPawn ? DebugPawn->GetController() : nullptr;
		const TInlineComponentArray<UUtilityAIComponent*> UtilityAIComponents(DebugController);
		if (UtilityAIComponents.Num() > 0)
		{
			const UUtilityAIComponent* UtilityAI = UtilityAIComponents[0];

			DataPack.CompName = UtilityAI->GetName();

			TArray<UUtilityAIAction*> AllActions;
			UtilityAI->GetAllActions(AllActions);
			AllActions.Sort([](const UUtilityAIAction& A, const UUtilityAIAction& B)
			{
				return A.Priority > B.Priority;
			});

			TArray<FString> Lines;
			for (const UUtilityAIAction* Action : AllActions)
			{
				FString Name = Action->GetName();

				const bool bMeetsTagRequirements = Action->AreTagRequirementsMet();
				const bool bIsExecuting = Action->IsExecuting();

				const FString Status = FString(
					bIsExecuting ? TEXT("(EXECUTING)") : !bMeetsTagRequirements ? TEXT("(TAGS NOT MET)") : TEXT(""));

				FString ActionLine = FString::Printf(TEXT("%s (P%d): %0.3f %s"), *Name, Action->Priority, Action->Score, *Status);
				Lines.Add(ActionLine);

				const int32 IntScore = Action->Score * 100;
				const FString ScoreBar = FString(TEXT("\t")) + FString::ChrN(IntScore, '|');
				Lines.Add(ScoreBar);
			}
			DataPack.ActionsDesc = FString::Join(Lines, TEXT("\n"));
		}
	}
}

void FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Printf(TEXT("Utility Component: {yellow}%s"), *DataPack.CompName);

	TArray<FString> ActionLines;
	DataPack.ActionsDesc.ParseIntoArrayLines(ActionLines, true);

	FColor Color = FColor::White;
	for (int32 Idx = 0; Idx < ActionLines.Num(); Idx += 2)
	{
		if (!ActionLines.IsValidIndex(Idx + 1))
		{
			// mismatch of lines, something went wrong
			break;
		}

		FString& ActionLine = ActionLines[Idx];
		FString BarLine = ActionLines[Idx + 1];

		const int32 BarScore = BarLine.Len() - 1;

		if (ActionLine.Contains(TEXT("(EXECUTING)")))
		{
			Color = FColor::Green;
		}
		else if (ActionLine.Contains(TEXT("(TAGS NOT MET)")) || BarScore == 0)
		{
			Color = FColor::Red;
		}
		else
		{
			float Score = BarScore / 100.f;
			Color = FMath::Lerp(FLinearColor(1.f, 0.5f, 0.f), FLinearColor::Yellow, Score).ToFColor(false);
		}

		CanvasContext.Printf(Color, TEXT("%s"), *ActionLine);
		CanvasContext.Printf(Color, TEXT("%s"), *BarLine);
	}
}

#endif
