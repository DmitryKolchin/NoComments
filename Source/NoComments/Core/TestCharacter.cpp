// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCharacter.h"
#include "MetahumanComponentDataExtractor/Components/MetahumanBuilderComponent.h"
// Sets default values
ATestCharacter::ATestCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "SkeletalMeshComponent" ) );
	MetahumanBuilderComponent = CreateDefaultSubobject<UMetahumanBuilderComponent>( TEXT( "MetahumanBuilderComponent" ) );


}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

void ATestCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction( Transform );

	MetahumanBuilderComponent->AddComponentsToOwner( SkeletalMeshComponent );
}

