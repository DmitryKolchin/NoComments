// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageDealingSphereComponent.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UDamageDealingSphereComponent::UDamageDealingSphereComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OnComponentBeginOverlap.AddDynamic( this,  &UDamageDealingSphereComponent::OnBeginOverlap );
	// ...
}

// Called every frame
void UDamageDealingSphereComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UDamageDealingSphereComponent::SetDealtDamage(float NewDamage)
{
	Damage = NewDamage;
}

// Called when the game starts
void UDamageDealingSphereComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>( GetOwner() );
	if (!IsValid( OwnerCharacter ))
	{
		ensureAlwaysMsgf( false, TEXT( "DamageDealingSphereComponent must be attached to a character" ) );
		return;
	}

	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if (!IsValid( OwnerMesh ))
	{
		ensureAlwaysMsgf( false, TEXT( "Owner character must have a skeletal mesh component" ) );
		return;
	}

	UAnimInstance* OwnerAnimInstance = OwnerMesh->GetAnimInstance();

	if (!IsValid( OwnerAnimInstance ))
	{
		ensureAlwaysMsgf( false, TEXT( "Owner character must have an anim instance" ) );
		return;
	}


	// Binding self-destruction to the end of the montage
	OwnerAnimInstance->OnMontageEnded.AddDynamic( this, &UDamageDealingSphereComponent::OnOwnerMontageEnded );

	// ...
}

void UDamageDealingSphereComponent::OnOwnerMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	DestroyComponent(  );
}

void UDamageDealingSphereComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	if (!IsValid( OtherActor ))
	{
		return;
	}

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage( Damage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner() );
	DestroyComponent(  );
}
