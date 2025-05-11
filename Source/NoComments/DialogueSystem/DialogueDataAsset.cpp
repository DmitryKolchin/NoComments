// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueDataAsset.h"

const TArray<FDialoguePhrase>& UDialogueDataAsset::GetPhrases()
{
	return Phrases;
}

const TArray<UDialogueActionBase*>& UDialogueDataAsset::GetActions()
{
	return Actions;
}
