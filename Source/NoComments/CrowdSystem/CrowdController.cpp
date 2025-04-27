// Fill out your copyright notice in the Description page of Project Settings.

#include "CrowdController.h"

#include "CrowdAgent.h"
#include "Algo/RandomShuffle.h"
#include "Components/MetahumanBuilderComponent.h"
#include "Components/SplineComponent.h"

ACrowdController::ACrowdController()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>( TEXT( "SplineComponent" ) );

	CrowdCenterPoint = CreateDefaultSubobject<USceneComponent>( TEXT("Center Point") );
}

void ACrowdController::GenerateCrowd()
{
	DestroyCreatedAgents();

	if ( !IsValid( GetWorld() ) )
	{
		return;
	}

	const float SplineLength = SplineComponent->GetSplineLength();
	const float SplineLengthStep = SplineLength / AgentsNum;

	TArray<UMetahumanComponentsDataAsset*> RandomizedPoolOfMetaHumanPresets = GetRandomizedPoolOfMetaHumanPresets();
	if ( RandomizedPoolOfMetaHumanPresets.IsEmpty() )
	{
		UE_LOG( LogTemp, Warning, TEXT( "RandomizedPoolOfMetaHumanPresets is empty" ) );
		return;
	}

	for ( int32 AgentIndex = 0; AgentIndex < AgentsNum; ++AgentIndex )
	{
		const float CurrentSplineLength = SplineLengthStep * AgentIndex;
		const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline( CurrentSplineLength, ESplineCoordinateSpace::World );

		const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline( CurrentSplineLength, ESplineCoordinateSpace::World );
		const FVector SplineDirection = SplineComponent->GetDirectionAtDistanceAlongSpline( CurrentSplineLength, ESplineCoordinateSpace::World );

		const float SplineDirectionRotationAngle = FMath::RandBool() ? 90.f : -90.f;
		FVector Offset = SplineDirection.RotateAngleAxis( SplineDirectionRotationAngle, FVector::UpVector ) * FMath::FRandRange( 0.f, MaxOffset );
		Offset.Z = AgentSpawnZOffset;

		ACrowdAgent* CrowdAgent = GetWorld()->SpawnActor<ACrowdAgent>( CrowdAgentClass, Location + Offset, Rotation );

		if ( !IsValid( CrowdAgent ) )
		{
			continue;
		}

		UMetahumanComponentsDataAsset* MetaHumanComponentsDataAsset = RandomizedPoolOfMetaHumanPresets.Pop();
		if ( RandomizedPoolOfMetaHumanPresets.IsEmpty() )
		{
			RandomizedPoolOfMetaHumanPresets = GetRandomizedPoolOfMetaHumanPresets();
		}

		CrowdAgent->GetMetahumanBuilderComponent()->SetMetahumanComponentsDataAsset( MetaHumanComponentsDataAsset );

		CrowdAgent->AttachToActor( this, FAttachmentTransformRules::KeepWorldTransform );
		CrowdAgents.Add( CrowdAgent );

		if ( CrowdAgents.IsValidIndex( AgentIndex - 1 ) )
		{
			ACrowdAgent* PreviousCrowdAgent = CrowdAgents[ AgentIndex - 1 ];
			CrowdAgent->AddNeighbor( PreviousCrowdAgent );
			PreviousCrowdAgent->AddNeighbor( CrowdAgent );
		}
	}

	if (AgentsNum < 2)
	{
		return;
	}

	ACrowdAgent* FirstCrowdAgent = CrowdAgents[ 0 ];
	ACrowdAgent* LastCrowdAgent = CrowdAgents.Last();
	if ( IsValid( FirstCrowdAgent ) && IsValid( LastCrowdAgent ) )
	{
		FirstCrowdAgent->AddNeighbor( LastCrowdAgent );
		LastCrowdAgent->AddNeighbor( FirstCrowdAgent );
	}
}

void ACrowdController::DestroyCreatedAgents()
{
	for ( auto CrowdAgent : CrowdAgents )
	{
		if ( !IsValid( CrowdAgent ) )
		{
			continue;
		}
		CrowdAgent->Destroy();
	}

	CrowdAgents.Empty();
}

TArray<UMetahumanComponentsDataAsset*> ACrowdController::GetRandomizedPoolOfMetaHumanPresets() const
{
	TArray<UMetahumanComponentsDataAsset*> RandomizedPoolOfMetaHumanPresets = CrowdAgentMetaHumanPresets;
	Algo::RandomShuffle( RandomizedPoolOfMetaHumanPresets );
	return RandomizedPoolOfMetaHumanPresets;
}
