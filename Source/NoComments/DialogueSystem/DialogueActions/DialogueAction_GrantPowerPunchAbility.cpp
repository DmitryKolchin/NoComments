// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueAction_GrantPowerPunchAbility.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NoComments/Characters/NCCharacter_Player.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

void UDialogueAction_GrantPowerPunchAbility::ExecuteAction(const UWorld* WorldContext)
{
	ANCCharacter_Player* PlayerCharacter = Cast<ANCCharacter_Player>( UGameplayStatics::GetPlayerCharacter( WorldContext, 0 ) );
	if ( !IsValid( PlayerCharacter ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Player character is not valid." ) );
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>( PlayerCharacter->InputComponent );
	EnhancedInputComponent->BindAction( PowerPunchAction, ETriggerEvent::Started, PlayerCharacter, &ANCCharacter_Player::PowerPunch );
}
