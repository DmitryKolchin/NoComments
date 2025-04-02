// Fill out your copyright notice in the Description page of Project Settings.

#include "CrowdAgent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

void ACrowdAgent::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->Deactivate();

	StartLocation = GetActorLocation();
}

FVector ACrowdAgent::GetVelocity() const
{
	return Velocity;
}

void ACrowdAgent::AddOffset(const FVector& Offset)
{
	AddActorWorldOffset( Offset );
	Velocity = Offset / GetWorld()->GetDeltaSeconds();
}

void ACrowdAgent::ResetVelocity()
{
	Velocity = FVector::ZeroVector;
}

void ACrowdAgent::AddNeighbor(ACrowdAgent* Neighbor)
{
	if (!IsValid( Neighbor ))
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Neighbor is not valid" ) );
		return;
	}

	Neighbors.Add( Neighbor );
}

const TArray<ACrowdAgent*>& ACrowdAgent::GetNeighbors() const
{
	return Neighbors;
}

FVector ACrowdAgent::GetStartLocation() const
{
	return StartLocation;
}

bool ACrowdAgent::IsAtStartLocation() const
{
	return StartLocation.Equals( GetActorLocation(), 1.f );
}
