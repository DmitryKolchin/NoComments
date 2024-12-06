// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPathpointManager.h"

#include "Components/BillboardComponent.h"

bool APatrolPathpointManager::ShouldTickIfViewportsOnly() const
{
	if ( IsValid( GetWorld() ) && GetWorld()->WorldType == EWorldType::Editor )
	{
		return true;
	}
	return false;
}

// Sets default values
APatrolPathpointManager::APatrolPathpointManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>( "Billboard" );
	SetRootComponent( Billboard );
}

// Called when the game starts or when spawned
void APatrolPathpointManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APatrolPathpointManager::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	for ( int32 PathpointIndex = 0.f; PathpointIndex < Pathpoints.Num(); ++PathpointIndex )
	{
		if ( !IsValid( Pathpoints[ PathpointIndex ] ) )
		{
			OnPathpointDestroyed( PathpointIndex );
			--PathpointIndex;
			continue;
		}

		TryBindPathpointEvents( Pathpoints[ PathpointIndex ] );

		FVector CurrentPathpointLocation = Pathpoints[ PathpointIndex ]->GetActorLocation();

		FVector LastPathpointLocation = Pathpoints.IsValidIndex( PathpointIndex - 1 ) && IsValid( Pathpoints[ PathpointIndex - 1 ] )
			                                ? Pathpoints[ PathpointIndex - 1 ]->GetActorLocation()
			                                : GetActorLocation();
		DrawDebugDirectionalArrow( GetWorld(), LastPathpointLocation, CurrentPathpointLocation, 10.f, LinesColor, false, 0.f, 0, LinesThickness );
	}

	if (PathType == EPatrolPathType::Loop && !Pathpoints.IsEmpty())
	{
		FVector LastPathpointLocation = Pathpoints.Last()->GetActorLocation();
		FVector PathStartLocation = GetActorLocation();
		DrawDebugDirectionalArrow( GetWorld(), LastPathpointLocation, PathStartLocation, 10.f, LinesColor, false, 0.f, 0, LinesThickness );
	}
}

void APatrolPathpointManager::Destroyed()
{
	Super::Destroyed();

	for (int32 PathpointIndex = 0; PathpointIndex < Pathpoints.Num(); ++PathpointIndex)
	{
		if ( IsValid( Pathpoints[ PathpointIndex ] ) )
		{
			Pathpoints[ PathpointIndex ]->Destroy();
		}
	}
}

void APatrolPathpointManager::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate( DuplicateMode );

	switch ( DuplicateMode )
	{
		case EDuplicateMode::PIE:
			break;
		case EDuplicateMode::World:
			break;
		case EDuplicateMode::Normal:
			// This is called when we are duplicating object in the editor
			Pathpoints.Empty();
			break;
	}
}

void APatrolPathpointManager::PostLoad()
{
	Super::PostLoad();

	if ( !IsValid( GetWorld() ) )
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick( this, &APatrolPathpointManager::RecalculateAllPathpointsIndexes );
}


FPatrolPathData APatrolPathpointManager::GetPatrolPathData() const
{
	FPatrolPathData PatrolPathData;

	PatrolPathData.PathType = PathType;

	PatrolPathData.PathpointLocations.Add( GetActorLocation() );

	for (auto Pathpoint : Pathpoints)
	{
		PatrolPathData.PathpointLocations.Add( Pathpoint->GetActorLocation() );
	}

	return PatrolPathData;
}

void APatrolPathpointManager::AddPathpointToEnd()
{
	// If pathpoints array is empty, we add a pathpoint to the start
	if ( Pathpoints.IsEmpty())
	{
		AddPathpointToStart();
		return;
	}

	// Otherwise we add a new pathpoint to the end of the path with the same offset as the last pathpoint
	FVector PreviousPathpointLocation = 1 < Pathpoints.Num() ? Pathpoints[ Pathpoints.Num() - 2 ]->GetActorLocation() : GetActorLocation();
	FVector CurrentPathpointLocation = Pathpoints[ Pathpoints.Num() - 1 ]->GetActorLocation();
	FVector Offset = CurrentPathpointLocation- PreviousPathpointLocation;

	AddPathpoint( CurrentPathpointLocation + Offset, Pathpoints.Num() );
}

void APatrolPathpointManager::AddPathpointToStart()
{
	FVector Offset = FVector{FMath::FRandRange( 0, 100.f ), 100.f, 0.f};
	AddPathpoint( GetActorLocation() + Offset, 0 );
}

void APatrolPathpointManager::AddPathpoint(const FVector& NewPathpointLocation, const int32 NewPathpointIndex)
{
	APatrolPathpoint* NewPathpoint = GetWorld()->SpawnActor<APatrolPathpoint>( PathpointClass, NewPathpointLocation, FRotator::ZeroRotator );

	TryBindPathpointEvents( NewPathpoint );

	if ( IsValid( NewPathpoint ) )
	{
		Pathpoints.Insert( NewPathpoint, NewPathpointIndex );
	}

	RecalculatePathpointsIndexesStartingFromIndex( NewPathpointIndex );
}

void APatrolPathpointManager::RecalculatePathpointsIndexesStartingFromIndex(int32 StartingIndex)
{
	for (int32 PathpointIndex = StartingIndex; PathpointIndex < Pathpoints.Num(); ++PathpointIndex)
	{
		if (Pathpoints.IsValidIndex( PathpointIndex ) && IsValid( Pathpoints[ PathpointIndex ] ))
		{
			Pathpoints[ PathpointIndex ]->SetPathpointIndex( PathpointIndex );
		}
	}
}

void APatrolPathpointManager::RecalculateAllPathpointsIndexes()
{
	RecalculatePathpointsIndexesStartingFromIndex( 0 );
}

void APatrolPathpointManager::OnAddForwardPathpointRequested(int32 PathpointIndex)
{
	if ( PathpointIndex == Pathpoints.Num() - 1 )
	{
		AddPathpointToEnd();
		return;
	}

	FVector CurrentPathpointLocation = Pathpoints[ PathpointIndex ]->GetActorLocation();
	FVector NextPathpointLocation = Pathpoints[ PathpointIndex + 1 ]->GetActorLocation();

	AddPathpoint( (CurrentPathpointLocation + NextPathpointLocation) / 2, PathpointIndex + 1 );
}

void APatrolPathpointManager::OnAddBackPathpointRequested(int32 PathpointIndex)
{
	if ( PathpointIndex == 0 )
	{
		AddPathpointToStart();
		return;
	}

	FVector CurrentPathpointLocation = Pathpoints[ PathpointIndex ]->GetActorLocation();
	FVector PreviousPathpointLocation = Pathpoints[ PathpointIndex - 1 ]->GetActorLocation();

	AddPathpoint( (CurrentPathpointLocation + PreviousPathpointLocation) / 2, PathpointIndex );
}

void APatrolPathpointManager::OnPathpointDestroyed(int32 PathpointIndex)
{
	Pathpoints.RemoveAt( PathpointIndex );
	RecalculatePathpointsIndexesStartingFromIndex( PathpointIndex );
}

void APatrolPathpointManager::TryBindPathpointEvents(APatrolPathpoint* Pathpoint)
{
	if (!IsValid( Pathpoint ))
	{
		return;
	}

	if (!Pathpoint->OnAddBackPathpointRequested.IsAlreadyBound( this, &APatrolPathpointManager::OnAddBackPathpointRequested ))
	{
		Pathpoint->OnAddBackPathpointRequested.AddDynamic( this, &APatrolPathpointManager::OnAddBackPathpointRequested );
	}

	if (!Pathpoint->OnAddForwardPathpointRequested.IsAlreadyBound( this, &APatrolPathpointManager::OnAddForwardPathpointRequested ))
	{
		Pathpoint->OnAddForwardPathpointRequested.AddDynamic( this, &APatrolPathpointManager::OnAddForwardPathpointRequested );
	}

}


