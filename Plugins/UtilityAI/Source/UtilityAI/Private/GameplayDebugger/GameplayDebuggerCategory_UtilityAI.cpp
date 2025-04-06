// Copyright Bohdon Sayre. All Rights Reserved.


#include "GameplayDebuggerCategory_UtilityAI.h"

#include "Engine/Canvas.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "UtilityAIComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


const FString FGameplayDebuggerCategory_UtilityAI::Status_ActiveBusy("(Active Busy)");
const FString FGameplayDebuggerCategory_UtilityAI::Status_Active("(Active)");
const FString FGameplayDebuggerCategory_UtilityAI::Status_TagsNotMet("(Tags Not Met)");
const FString FGameplayDebuggerCategory_UtilityAI::Status_NoScore("(No Score)");
const FString FGameplayDebuggerCategory_UtilityAI::Status_Considering("(Considering)");


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
	if (!DebugActor)
	{
		return;
	}

	const APawn* DebugPawn = Cast<APawn>(DebugActor);
	const AController* DebugController = DebugPawn ? DebugPawn->GetController() : nullptr;
	const TInlineComponentArray<UUtilityAIComponent*> UtilityAIComponents(DebugController);
	if (UtilityAIComponents.IsEmpty())
	{
		return;
	}

	const UUtilityAIComponent* UtilityAI = UtilityAIComponents[0];

	DataPack.CompName = UtilityAI->GetReadableName();

	// add all actions and their scores
	TArray<UUtilityAIAction*> AllActions = UtilityAI->GetAllActions();
	AllActions.Sort([](const UUtilityAIAction& A, const UUtilityAIAction& B)
	{
		return A.ScoreWeight > B.ScoreWeight;
	});

	// normalize bars to max score
	float MaxScore = 1.f;
	for (const UUtilityAIAction* Action : AllActions)
	{
		MaxScore = FMath::Max(MaxScore, Action->ScoreWeight);
	}

	TArray<FString> Lines;
	for (const UUtilityAIAction* Action : AllActions)
	{
		FString Name = Action->GetClass()->GetName();
		Name.RemoveFromEnd(TEXT("_C"));

		// determine the status of the action.
		FString ExecutingStr("       ");
		FString Status;
		if (Action->IsExecuting())
		{
			ExecutingStr = TEXT(">>>");
			if (UtilityAI->IsBusy())
			{
				Status = Status_ActiveBusy;
			}
			else
			{
				Status = Status_Active;
			}
		}
		else if (!Action->AreTagRequirementsMet())
		{
			Status = Status_TagsNotMet;
		}
		else if (Action->GetScore() <= UE_SMALL_NUMBER)
		{
			Status = Status_NoScore;
		}
		else
		{
			Status = Status_Considering;
		}

		const FUtilityAIScoringElements& ScoringElements = Action->GetScoringElements();

		// show action name and state: ">>> My Action (BUSY)
		FString ActionLine = FString::Printf(TEXT("%s %s %s"), *ExecutingStr, *Name, *Status);
		Lines.Add(ActionLine);

		if (!Action->AreTagRequirementsMet())
		{
			continue;
		}

		// add score meter: "[||||       ] [12.0 / 20.0]"
		const FString ScoreStr = FString::Printf(TEXT("%.2f / %.2f"), Action->GetScore(), Action->ScoreWeight);
		const FString ScoreBarStr = FormatScoreBar(Action->GetScore(), Action->ScoreWeight, MaxScore);
		const FString OperationStr = StaticEnum<EUtilityAIScoreOperation>()->GetNameStringByValue(static_cast<int64>(ScoringElements.Operation));
		const FString ScoreBarLine = FString::Printf(TEXT("\t\t%s [%s] (%s)"), *ScoreBarStr, *ScoreStr, *OperationStr);
		Lines.Add(ScoreBarLine);

		// display element scores
		const int32 ActionScoreBarWidth = FMath::CeilToInt((Action->ScoreWeight / MaxScore) * 100);
		for (int32 ElementIdx = 0; ElementIdx < ScoringElements.Scores.Num(); ++ElementIdx)
		{
			check(ScoringElements.Names.IsValidIndex(ElementIdx));

			const float ElementScore = ScoringElements.Scores[ElementIdx];
			const FString ElementName = ScoringElements.Names[ElementIdx];

			const FString ElemScoreBarStr = FormatProgressBar(ElementScore, ActionScoreBarWidth, '\'');
			FString ElemScoreLine = FString::Printf(TEXT("\t\t%s [%.2f] %s"), *ElemScoreBarStr, ElementScore, *ElementName);
			Lines.Add(ElemScoreLine);
		}
	}
	DataPack.ActionsDesc = FString::Join(Lines, TEXT("\n"));
}

void FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	const float CursorYStart = CanvasContext.CursorY;

	if (LastDrawDataHeight <= 0.0f)
	{
		// no height info yet, fill the canvas
		LastDrawDataHeight = CanvasContext.Canvas->SizeY - CursorYStart;
	}

	// draw transparent background to make text easier to read
	const FLinearColor BGColor(0.1f, 0.1f, 0.1f, 0.8f);
	const FVector2D BGPos(CanvasContext.CursorX, CanvasContext.CursorY);
	const float BGMaxWidth = CanvasContext.Canvas->SizeX - (2.0f * CanvasContext.CursorX);
	const FVector2D BGSize(FMath::Min(BGMaxWidth, 600), LastDrawDataHeight);

	FCanvasTileItem Background(FVector2D(0.0f), BGSize, BGColor);
	Background.BlendMode = SE_BLEND_Translucent;
	CanvasContext.DrawItem(Background, BGPos.X, BGPos.Y);

	// small padding
	CanvasContext.CursorY += CanvasContext.GetLineHeight() * 0.5f;


	CanvasContext.Printf(TEXT("Utility Component: {yellow}%s"), *DataPack.CompName);

	CanvasContext.Printf(TEXT("Actions:"));
	TArray<FString> ActionLines;
	DataPack.ActionsDesc.ParseIntoArrayLines(ActionLines, true);

	FString ColorStr = TEXT("{white}");
	for (int32 Idx = 0; Idx < ActionLines.Num(); ++Idx)
	{
		const FString& Line = ActionLines[Idx];

		if (Line.Contains(Status_ActiveBusy))
		{
			ColorStr = TEXT("{cyan}");
		}
		else if (Line.Contains(Status_Active))
		{
			ColorStr = TEXT("{green}");
		}
		else if (Line.Contains(Status_TagsNotMet))
		{
			ColorStr = TEXT("{red}");
		}
		else if (Line.Contains(Status_NoScore))
		{
			ColorStr = TEXT("{grey}");
		}
		else if (Line.Contains(Status_Considering))
		{
			ColorStr = TEXT("{white}");
		}

		CanvasContext.Printf(TEXT("%s%s"), *ColorStr, *Line);
	}

	// small padding
	CanvasContext.CursorY += CanvasContext.GetLineHeight() * 0.5f;

	LastDrawDataHeight = CanvasContext.CursorY - CursorYStart;
}

FString FGameplayDebuggerCategory_UtilityAI::FormatScoreBar(float Score, float ScoreWeight, float MaxScore)
{
	const int32 BarWidth = FMath::CeilToInt((ScoreWeight / MaxScore) * 100);
	const float Percent = Score / ScoreWeight;
	return FormatProgressBar(Percent, BarWidth, '|');
}

FString FGameplayDebuggerCategory_UtilityAI::FormatProgressBar(float Percent, int32 BarWidth, TCHAR Char)
{
	const int32 NumBars = FMath::Clamp(FMath::CeilToInt(Percent * BarWidth), 0, BarWidth);
	const FString Bar = FString::ChrN(NumBars, Char).RightPad(BarWidth);
	const FString Result = FString::Printf(TEXT("[%s]"), *Bar);
	return Result;
}

#endif
