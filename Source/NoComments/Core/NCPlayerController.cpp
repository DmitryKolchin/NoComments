// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerController.h"

ANCPlayerController::ANCPlayerController(const FObjectInitializer& ObjectInitializer) :
	Super( ObjectInitializer )
{
	TeamId = FGenericTeamId{2};
}

void ANCPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if ( NewTeamID != TeamId )
	{
		TeamId = NewTeamID;
	}
}

FGenericTeamId ANCPlayerController::GetGenericTeamId() const
{
	return TeamId;
}
