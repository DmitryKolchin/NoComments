// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueComponent.h"

#include "DialogueDataAsset.h"
#include "DialogueWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "DialogueActions/DialogueActionBase.h"
#include "Kismet/GameplayStatics.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

bool UDialogueComponent::HasAvailableDialogues() const
{
	return !DialogueQueue.IsEmpty();
}

void UDialogueComponent::PlayCurrentAvailableDialogue()
{
	if ( !HasAvailableDialogues() )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "No available dialogues to play." ) );
		return;
	}

	CurrentPhraseIndex = 0;

	DialogueWidget = CreateWidget<UDialogueWidgetBase>( GetWorld(), DialogueWidgetClass );
	if (!IsValid( DialogueWidget ))
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Dialogue widget is not valid." ) );
		return;
	}

	DialogueWidget->OnPhraseFinished.AddDynamic( this, &UDialogueComponent::PostPhraseFinished );
	DialogueWidget->AddToViewport();

	PlayNextDialoguePhrase();




}

void UDialogueComponent::SkipCurrentPhrase()
{
	if ( !IsValid( DialogueWidget ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Dialogue widget is not valid." ) );
		return;
	}

	DialogueWidget->ForceFinishActivePhrase();

	PostPhraseFinished();
}

void UDialogueComponent::PostPhraseFinished()
{
	++CurrentPhraseIndex;

	if ( IsValid( CurrentVoiceOverAudioComponent ) )
	{
		CurrentVoiceOverAudioComponent->Stop();
		CurrentVoiceOverAudioComponent->DestroyComponent();
	}

	if (DialogueQueue.Top()->GetPhrases().Num() <= CurrentPhraseIndex)
	{
		PostDialogueFinished();
		return;
	}

	PlayNextDialoguePhrase();
}

void UDialogueComponent::PlayNextDialoguePhrase()
{
	UDialogueDataAsset* CurrentDialogue = DialogueQueue.Top();
	{
		if ( !IsValid( CurrentDialogue ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Current dialogue is not valid." ) );
			return;
		}
		if (!CurrentDialogue->GetPhrases().IsValidIndex( CurrentPhraseIndex ))
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Current phrase index is out of range." ) );
			return;
		}
		if (!IsValid( DialogueWidget ))
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Dialogue widget is not valid." ) );
			return;
		}
	}

	FDialoguePhrase DialoguePhraseToPlay = DialogueQueue.Top()->GetPhrases()[ CurrentPhraseIndex ];

	DialogueWidget->PlayPhrase( DialoguePhraseToPlay.Author, DialoguePhraseToPlay.PhraseText );

	if ( IsValid( DialoguePhraseToPlay.VoiceOver.LoadSynchronous() ) )
	{
		PlayDialogueVoiceOver( DialoguePhraseToPlay.VoiceOver.LoadSynchronous() );
	}
	if ( IsValid( DialoguePhraseToPlay.TalkAnimation.LoadSynchronous() ) )
	{
		PlayDialogueAnimation( DialoguePhraseToPlay.TalkAnimation.LoadSynchronous() );
	}
}

void UDialogueComponent::PostDialogueFinished()
{
	if ( !IsValid( DialogueWidget ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Dialogue widget is not valid." ) );
		return;
	}
	DialogueWidget->RemoveFromParent();

	TArray<UDialogueActionBase*> Actions = DialogueQueue.Top()->GetActions();

	for ( auto Action : Actions )
	{
		if ( Action )
		{
			Action->ExecuteAction( GetWorld() );
		}
		else
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Action is null." ) );
			return;
		}
	}

	DialogueQueue.Pop();
	OnDialogueFinished.Broadcast();
}

void UDialogueComponent::PlayDialogueVoiceOver(USoundBase* VoiceOver)
{
	if ( !IsValid( GetOwner() ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Owner is not valid." ) );
		return;
	}

	USkeletalMeshComponent* OwnerSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if ( !IsValid( OwnerSkeletalMesh ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Mesh is not valid." ) );
		return;
	}

	FVector VoiceOverSoundLocation = OwnerSkeletalMesh->GetBoneLocation( DialogueVoiceOverAttachmentBoneName, EBoneSpaces::Type::WorldSpace );

	CurrentVoiceOverAudioComponent = UGameplayStatics::SpawnSoundAtLocation( GetWorld(), VoiceOver, VoiceOverSoundLocation );
}

void UDialogueComponent::PlayDialogueAnimation(UAnimMontage* Animation)
{
	if ( !IsValid( GetOwner() ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Owner is not valid." ) );
		return;
	}

	USkeletalMeshComponent* OwnerSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if ( !IsValid( OwnerSkeletalMesh ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Mesh is not valid." ) );
		return;
	}

	UAnimInstance* AnimInstance = OwnerSkeletalMesh->GetAnimInstance();
	if ( !IsValid( AnimInstance ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Anim instance is not valid." ) );
		return;
	}
	AnimInstance->Montage_Play( Animation );
}
