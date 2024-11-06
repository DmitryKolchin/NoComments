// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerCharacter.h"

// Sets default values
ANCPlayerCharacter::ANCPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANCPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANCPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ANCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
}

void ANCPlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	return;
}

FGenericTeamId ANCPlayerCharacter::GetGenericTeamId() const
{
	return TeamId;
}

void ANCPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy( NewController );

	if ( IGenericTeamAgentInterface* ControllerAsTeamAgent = Cast<IGenericTeamAgentInterface>( NewController ) )
	{
		TeamId = ControllerAsTeamAgent->GetGenericTeamId();
	}
}

