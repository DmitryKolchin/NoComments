// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "DamageDealingSphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NoComments/DataAssets/CombatSettingsDataAsset.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Soft lock logic
	if ( IsValid( TargetEnemy ) )
	{
		{
			if ( !IsValid( GetOwner() ) )
			{
				UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "Owner is not valid" );
				return;
			}
		}

		//TODO: This thing is dangerous as hell because there are more than 1 skeletal mesh components in the owner actor
		USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

		{
			if ( !IsValid( OwnerSkeletalMeshComponent ) )
			{
				UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerSkeletalMeshComponent is not valid" );
				return;
			}
		}

		UAnimInstance* OwnerAnimInstance = OwnerSkeletalMeshComponent->GetAnimInstance();

		{
			if ( !IsValid( OwnerAnimInstance ) )
			{
				UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
				return;
			}
		}

		// Soft lock is not supposed to work during any montage. otherwise the character will rotate to the target enemy
		// in the middle of the attack animation
		if ( OwnerAnimInstance->IsAnyMontagePlaying() )
		{
			return;
		}

		FRotator NewOwnerRotation = GetOwner()->GetActorRotation();
		NewOwnerRotation.Yaw = UKismetMathLibrary::FindLookAtRotation( GetOwner()->GetActorLocation(), TargetEnemy->GetActorLocation() ).Yaw;

		// TODO: Make rotation speed adjustable
		GetOwner()->SetActorRotation( NewOwnerRotation );
	}
}

void UCombatComponent::ActivateFightMode(AActor* NewTargetEnemy)
{
	{
		if ( !IsValid( NewTargetEnemy ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "NewTargetEnemy is not valid" );
			return;
		}
	}

	TargetEnemy = NewTargetEnemy;
	SetOwnerWalkSpeed_FightModeDefault();
}

void UCombatComponent::DeactivateFightMode()
{
	TargetEnemy = nullptr;

	// Let the walk speed change be the responsibility of the caller
}

void UCombatComponent::ActivateBlock()
{
	bBlockActive = true;
	SetOwnerWalkSpeed_FightModeBlock();
}

void UCombatComponent::DeactivateBlock()
{
	bBlockActive = false;
	SetOwnerWalkSpeed_FightModeDefault();
}

bool UCombatComponent::IsBlockActive() const
{
	return bBlockActive;
}

void UCombatComponent::PlayAttackMontage(const FName& DamageDealingComponentSocketName, TSoftObjectPtr<UAnimMontage> MontageToPlay)
{
	{
		if ( !IsValid( MontageToPlay.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "MontageToPlay is not valid" );
			return;
		}

		if ( !IsValid( CombatSettings.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CombatSettings is not valid" );
			return;
		}
	}

	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	{
		if ( !IsValid( OwnerAnimInstance ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
			return;
		}
	}

	// If we are already playing a montage, we should not interrupt it
	//TODO: Consider adding a queue for montages
	if ( OwnerAnimInstance->IsAnyMontagePlaying() )
	{
		return;
	}

	OwnerAnimInstance->Montage_Play( MontageToPlay.LoadSynchronous(), CombatSettings.LoadSynchronous()->GetAttackSpeed() );

	UDamageDealingSphereComponent* DamageDealingSphereComponent = NewObject<UDamageDealingSphereComponent>( GetOwner() );

	// Don't need to check for validity as we've already checked it when getting anim instance
	USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	DamageDealingSphereComponent->SetupAttachment( OwnerSkeletalMeshComponent, DamageDealingComponentSocketName );
	DamageDealingSphereComponent->RegisterComponent();
	DamageDealingSphereComponent->SetRelativeLocation( FVector::ZeroVector );

	//TODO: Remove the magic number
	DamageDealingSphereComponent->SetSphereRadius( 10.f, true );
}

void UCombatComponent::SetOwnerWalkSpeed(float NewWalkSpeed)
{
	UCharacterMovementComponent* OwnerCharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();

	{
		if ( !IsValid( OwnerCharacterMovementComponent ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerCharacterMovementComponent is not valid" );
			return;
		}
	}

	OwnerCharacterMovementComponent->MaxWalkSpeed = NewWalkSpeed;
}

void UCombatComponent::SetOwnerWalkSpeed_FightModeDefault()
{
	{
		if ( !IsValid( CombatSettings.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CombatSettings is not set" );
			return;
		}
	}

	SetOwnerWalkSpeed( CombatSettings.LoadSynchronous()->GetIdleWalkSpeed() );
}

void UCombatComponent::SetOwnerWalkSpeed_FightModeBlock()
{
	{
		if ( !IsValid( CombatSettings.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CombatSettings is not set" );
			return;
		}
	}

	SetOwnerWalkSpeed( CombatSettings.LoadSynchronous()->GetBlockWalkSpeed() );
}

UAnimInstance* UCombatComponent::GetOwnerAnimInstance() const
{
	{
		if ( !IsValid( GetOwner() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "Owner is not valid" );
			return nullptr;
		}
	}

	USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	{
		if ( !IsValid( OwnerSkeletalMeshComponent ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerSkeletalMeshComponent is not valid" );
			return nullptr;
		}
	}

	return OwnerSkeletalMeshComponent->GetAnimInstance();
}
