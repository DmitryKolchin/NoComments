// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueActionBase.h"

#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

void UDialogueActionBase::ExecuteAction(const UWorld* WorldContext)
{
	UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "This function should be overridden in the derived class." ) );
	return;
}
