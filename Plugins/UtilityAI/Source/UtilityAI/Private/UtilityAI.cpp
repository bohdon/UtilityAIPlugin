// Copyright Epic Games, Inc. All Rights Reserved.

#include "UtilityAI.h"


#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "GameplayDebugger/GameplayDebuggerCategory_UtilityAI.h"
#endif

DEFINE_LOG_CATEGORY(LogUtilityAI)

#define LOCTEXT_NAMESPACE "FUtilityAIModule"

void FUtilityAIModule::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory(
		"UtilityAI", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_UtilityAI::MakeInstance),
		EGameplayDebuggerCategoryState::EnabledInGameAndSimulate);
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FUtilityAIModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("UtilityAI");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAIModule, UtilityAI)
