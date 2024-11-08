// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathpoint.h"

#include "Components/BillboardComponent.h"

// Sets default values
APatrolPathpoint::APatrolPathpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>( TEXT("Billboard") );
	SetRootComponent( Billboard );

}

// Called when the game starts or when spawned
void APatrolPathpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void APatrolPathpoint::SetPathpointIndex(int32 NewPathpointIndex)
{
	PathpointIndex = NewPathpointIndex;
	UpdatePathpointIndexText(PathpointIndex);
}

void APatrolPathpoint::AddPathpointForward()
{
	OnAddForwardPathpointRequested.Broadcast(PathpointIndex);
}

void APatrolPathpoint::AddPathpointBackward()
{
	OnAddBackPathpointRequested.Broadcast(PathpointIndex);
}

// Called every frame
void APatrolPathpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

