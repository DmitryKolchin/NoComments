// Fill out your copyright notice in the Description page of Project Settings.

#include "NCCharacter_NPC_Enemy.h"

#include "NoComments/Components/CombatComponent.h"

// Sets default values
ANCCharacter_NPC_Enemy::ANCCharacter_NPC_Enemy()
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>( TEXT( "CombatComponent" ) );
}

UCombatComponent* ANCCharacter_NPC_Enemy::GetCombatComponent() const
{
	return CombatComponent;
}
