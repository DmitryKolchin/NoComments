// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DialogueComponent.generated.h"

class UDialogueWidgetBase;
class UDialogueDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FDialogueComponentDelegate );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOCOMMENTS_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	UPROPERTY(EditInstanceOnly, Category="Dialogue")
	TArray<TObjectPtr<UDialogueDataAsset>> DialogueQueue;

	UPROPERTY(EditDefaultsOnly, Category="Dialogue")
	FName DialogueVoiceOverAttachmentBoneName = "head";


	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UDialogueWidgetBase> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<UDialogueWidgetBase> DialogueWidget;

	UPROPERTY()
	int32 CurrentPhraseIndex = 0;

	UPROPERTY()
	UAudioComponent* CurrentVoiceOverAudioComponent = nullptr;


public:
	UPROPERTY(BlueprintAssignable)
	FDialogueComponentDelegate OnDialogueFinished;

public:
	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Has Available Dialogues" ) bool HasAvailableDialogues() const;

	UFUNCTION(BlueprintCallable)
	void PlayCurrentAvailableDialogue();

	UFUNCTION(BlueprintCallable)
	void SkipCurrentPhrase();

private:
	UFUNCTION()
	void PostPhraseFinished();

	void PlayNextDialoguePhrase();

	void PostDialogueFinished();

	void PlayDialogueVoiceOver(USoundBase* VoiceOver);

	void PlayDialogueAnimation(UAnimMontage* Animation);


};
