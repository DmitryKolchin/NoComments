// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FDialogueWidgetDelegate );

/**
 *  Base class for the dialogue widget
 */
UCLASS()
class NOCOMMENTS_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable)
	FDialogueWidgetDelegate OnPhraseFinished;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPhrase(const FText& Author, const FText& PhraseText);

	UFUNCTION(BlueprintImplementableEvent)
	void ForceFinishActivePhrase();
};
