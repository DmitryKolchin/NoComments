// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

class UDialogueActionBase;

USTRUCT(BlueprintType)
struct FDialoguePhrase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Author;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PhraseText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> VoiceOver;

	/**Currently only for the NPC */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> TalkAnimation;
};

/**
 *
 */
UCLASS()
class NOCOMMENTS_API UDialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category="Phrases")
	TArray<FDialoguePhrase> Phrases;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<UDialogueActionBase*> Actions;

public:
	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Phrases" )const TArray<FDialoguePhrase>& GetPhrases();

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Actions" )const TArray<UDialogueActionBase*>& GetActions();
};
