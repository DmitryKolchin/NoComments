// Fill out your copyright notice in the Description page of Project Settings.

#include "NCCharacter_Player.h"

#include "NoComments/Components/CombatComponent.h"

ANCCharacter_Player::ANCCharacter_Player()
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>( TEXT( "CombatComponent" ) );
}

void ANCCharacter_Player::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	return;
}

FGenericTeamId ANCCharacter_Player::GetGenericTeamId() const
{
	return TeamId;
}

void ANCCharacter_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy( NewController );

	if ( IGenericTeamAgentInterface* ControllerAsTeamAgent = Cast<IGenericTeamAgentInterface>( NewController ) )
	{
		TeamId = ControllerAsTeamAgent->GetGenericTeamId();
	}
}

UCombatComponent* ANCCharacter_Player::GetCombatComponent() const
{
	return CombatComponent;
}
