// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueActionBase.generated.h"

/**
 *  Base class for all the dialogue actions.
 */
UCLASS( Abstract, Blueprintable, EditInlineNew, DefaultToInstanced, BlueprintType )
class NOCOMMENTS_API UDialogueActionBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ExecuteAction(const UWorld* WorldContext);
};
