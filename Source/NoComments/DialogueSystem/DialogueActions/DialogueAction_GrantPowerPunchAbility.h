// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueActionBase.h"
#include "DialogueAction_GrantPowerPunchAbility.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class NOCOMMENTS_API UDialogueAction_GrantPowerPunchAbility : public UDialogueActionBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	UInputAction* PowerPunchAction;

	public:
	virtual void ExecuteAction(const UWorld* WorldContext) override;
};
