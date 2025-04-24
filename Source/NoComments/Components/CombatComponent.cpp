// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "DamageDealingSphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NoComments/DataAssets/CombatSettingsDataAsset.h"
#include "NoComments/DataAssets/StunAnimationsDataAsset.h"
#include "NoComments/DataStructures/Structs/AttackData.h"
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

	{
		if (!IsValid( GetOwner() ))
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "No owner" );
			return;
		}
	}

	GetOwner()->OnTakeAnyDamage.AddDynamic( this, &UCombatComponent::OnOwnerTakeDamage );

	UCombatSettingsDataAsset* CombatSettingsAsset = CombatSettings.LoadSynchronous();

	{
		if ( !IsValid( CombatSettingsAsset ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CombatSettings is not valid" );
			return;
		}
	}

	CurrentHealth = CombatSettingsAsset->GetMaxHealth();
	CurrentStamina = CombatSettingsAsset->GetMaxStamina();
	// ...
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Soft lock logic
	if ( CanSoftLockOnOpponent() )
	{
		SoftLockOnOpponent();
	}
}

void UCombatComponent::ActivateFightMode(AActor* NewOpponent)
{
	{
		if ( !IsValid( NewOpponent ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "NewTargetEnemy is not valid" );
			return;
		}
	}

	Opponent = NewOpponent;
	SetOwnerWalkSpeed_FightModeDefault();
}

void UCombatComponent::DeactivateFightMode()
{
	Opponent = nullptr;

	// Let the walk speed change be the responsibility of the caller
}

void UCombatComponent::ActivateBlock()
{
	CharacterCombatState = ECharacterCombatState::Blocking;
	NumberOfAttackTakenBeforeBlock = 0;
	SetOwnerWalkSpeed_FightModeBlock();
}

void UCombatComponent::DeactivateBlock()
{
	CharacterCombatState = ECharacterCombatState::Idle;
	SetOwnerWalkSpeed_FightModeDefault();
}

ECharacterCombatState UCombatComponent::GetCharacterCombatState() const
{
	return CharacterCombatState;
}

AActor* UCombatComponent::GetTargetOpponent() const
{
	return Opponent;
}

int32 UCombatComponent::GetNumberOfAttackTakenBeforeBlock() const
{
	return NumberOfAttackTakenBeforeBlock;
}

float UCombatComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UCombatComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

bool UCombatComponent::CanPerformAnyAttack() const
{

	// We can not attack when we are knocked out
	if (CharacterCombatState == ECharacterCombatState::KnockedOut)
	{
		return false;
	}

	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	{
		if ( !IsValid( OwnerAnimInstance ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
			return false;
		}
	}

	// If we are in the middle of an attack, we can't do another one
	return !OwnerAnimInstance->IsAnyMontagePlaying();
}

bool UCombatComponent::CanPerformGivenAttack(const FAttackData& AttackData) const
{
	// If we cannot perform any attack, we cannot perform this one
	if ( !CanPerformAnyAttack() )
	{
		return false;
	}

	// If we don't have enough stamina, we cannot attack
	if ( GetCurrentStamina() < AttackData.StaminaCost )
	{
		return false;
	}

	return true;
}

void UCombatComponent::Attack(const FAttackData& AttackData)
{
	{
		if ( !IsValid( AttackData.AttackAnimationMontage.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "MontageToPlay is not valid" );
			return;
		}

		if ( !IsValid( CombatSettings.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CombatSettings is not valid" );
			return;
		}

		if ( CurrentStamina < AttackData.StaminaCost )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CurrentStamina is less than 0" );
			CurrentStamina = 0.f;
			return;
		}

		if ( !CanPerformGivenAttack( AttackData ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "Cannot perform given attack" );
			return;
		}
	}

	ActiveAttackData = AttackData;

	CurrentStamina -= ActiveAttackData.StaminaCost;
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	{
		if ( !IsValid( OwnerAnimInstance ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
			return;
		}
	}

	// Very strange bug: this function is called twice when play first time after restarting the game
	if ( !OwnerAnimInstance->OnMontageEnded.IsAlreadyBound( this, &UCombatComponent::PerformPostAttackFinishedActions ) )
	{
		OwnerAnimInstance->OnMontageEnded.AddDynamic( this, &UCombatComponent::PerformPostAttackFinishedActions );
	}

	OwnerAnimInstance->Montage_Play( ActiveAttackData.AttackAnimationMontage.LoadSynchronous(), CombatSettings.LoadSynchronous()->GetAttackSpeed() );

	if ( CharacterCombatState == ECharacterCombatState::Blocking )
	{
		// If we are blocking, we need to deactivate the block
		DeactivateBlock();
	}

	CharacterCombatState = ECharacterCombatState::Attacking;

	// Prevent stamina from being restored for a time after attack
	if ( StaminaRestoreTimerHandle.IsValid() )
	{
		StaminaRestoreTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer( StaminaRestoreTimerHandle,
	                                        this,
	                                        &UCombatComponent::RestoreStamina,
	                                        CombatSettings.LoadSynchronous()->GetStaminaAfterAttackRestoreDelay() );
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

void UCombatComponent::PerformPostAttackFinishedActions(UAnimMontage* FinishedAttackMontage, bool bInterrupted)
{
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	{
		if ( !IsValid( OwnerAnimInstance ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
			return;
		}
	}

	OwnerAnimInstance->OnMontageEnded.RemoveDynamic( this, &UCombatComponent::PerformPostAttackFinishedActions );

	CharacterCombatState = ECharacterCombatState::Idle;

	OnAttackFinished.Broadcast();
}

void UCombatComponent::PerformPostStunFinishedActions(UAnimMontage* FinishedStunMontage, bool bInterrupted)
{
	// Checking if the callback is called for the same stun montage
	if (FinishedStunMontage != ActiveStunMontage)
	{
		return;
	}

	ActiveStunMontage = nullptr;
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	{
		if ( !IsValid( OwnerAnimInstance ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
			return;
		}
	}

	OwnerAnimInstance->OnMontageEnded.RemoveDynamic( this, &UCombatComponent::PerformPostStunFinishedActions );
	CharacterCombatState = ECharacterCombatState::Idle;
	OnOwnerStunFinished.Broadcast();
}

void UCombatComponent::OnOwnerTakeDamage(AActor* DamagedActor,
                                         float Damage,
                                         const class UDamageType* DamageType,
                                         class AController* InstigatedBy,
                                         AActor* DamageCauser)
{
	if ( GetCharacterCombatState() == ECharacterCombatState::Blocking )
	{
		bool bSuccessfullyBlocked = TryBlockAttack( Damage );
		if ( bSuccessfullyBlocked )
		{
			return;
		}
	}

	// Track number of attacks taken before block
	++NumberOfAttackTakenBeforeBlock;

	CurrentHealth -= Damage;

	// Track death
	if ( CurrentHealth <= 0.f )
	{
		KnockOutOwner();
		return;
	}

	FStunAnimationData StunAnimationData = GetStunAnimationDataForDamage( Damage );

	ActiveStunMontage = StunAnimationData.StunAnimation.LoadSynchronous();

	{
		if ( !IsValid( ActiveStunMontage ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "ActiveStunMontage is not valid" );
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

	OwnerAnimInstance->Montage_Play( ActiveStunMontage, StunAnimationData.GetStunAnimationPlayRate() );

	if ( !OwnerAnimInstance->OnMontageEnded.IsAlreadyBound( this, &UCombatComponent::PerformPostStunFinishedActions ) )
	{
		OwnerAnimInstance->OnMontageEnded.AddDynamic( this, &UCombatComponent::PerformPostStunFinishedActions );
	}

	CharacterCombatState = ECharacterCombatState::Stunned;

	UCharacterMovementComponent* OwnerCharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	{
		if ( !IsValid( OwnerCharacterMovementComponent ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerCharacterMovementComponent is not valid" );
			return;
		}
	}
	ActiveKnockBackTargetTime = StunAnimationData.KnockBackDistance / OwnerCharacterMovementComponent->MaxWalkSpeed;
	ActiveKnockBackPassedTime = 0.f;
	AddKnockBackImpulsePerTick();
}

FStunAnimationData UCombatComponent::GetStunAnimationDataForDamage(float Damage) const
{
	UStunAnimationsDataAsset* StunAnimationsDataAsset = CombatSettings.LoadSynchronous()->GetStunAnimationsDataAsset();

	{
		if ( !IsValid( StunAnimationsDataAsset ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "StunAnimationsDataAsset is not valid" );
			return FStunAnimationData{};
		}
	}

	// Getting stun animations sorted by the min required damage
	TArray<FStunAnimationData> StunAnimations = StunAnimationsDataAsset->GetStunAnimations();
	StunAnimations.Sort( [](const FStunAnimationData& A, const FStunAnimationData& B)
	{
		return A.MinDamageRequired < B.MinDamageRequired;
	} );

	// TODO: Binary search for the first animation that has min damage required less than the damage
	for ( int32 StunAnimationIndex = 0; StunAnimationIndex < StunAnimations.Num(); ++StunAnimationIndex )
	{
		const FStunAnimationData& CurrentStunAnimation = StunAnimations[ StunAnimationIndex ];

		// If we pass the damage check, we can go further
		if ( CurrentStunAnimation.MinDamageRequired <= Damage )
		{
			continue;
		}

		// If we are here, it means that the damage is less than the minimum damage required for this.
		if ( StunAnimationIndex == 0 )
		{
			// If this is the first animation, we can return it
			return CurrentStunAnimation;
		}

		// Otherwise, we can return the previous animation
		return StunAnimations[ StunAnimationIndex - 1 ];
	}

	return StunAnimations.Last();
}

void UCombatComponent::AddKnockBackImpulsePerTick()
{
	ActiveKnockBackPassedTime += GetWorld()->GetDeltaSeconds();

	if ( ActiveKnockBackTargetTime <= ActiveKnockBackPassedTime )
	{
		// If we are here, it means that the knockback is finished
		return;
	}

	APawn* OwnerPawn = Cast<APawn>( GetOwner() );

	{
		if ( !IsValid( OwnerPawn ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerPawn is not valid" );
			return;
		}
	}

	OwnerPawn->AddMovementInput( OwnerPawn->GetActorForwardVector(), -1.f );

	GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UCombatComponent::AddKnockBackImpulsePerTick );
}

void UCombatComponent::SpawnDamageDealingSphereForActiveAttack()
{
	UDamageDealingSphereComponent* DamageDealingSphereComponent = NewObject<UDamageDealingSphereComponent>( GetOwner() );

	// Don't need to check for validity as we've already checked it when getting anim instance
	USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	// Setting up the DamageDealingSphereComponent
	DamageDealingSphereComponent->SetupAttachment( OwnerSkeletalMeshComponent, ActiveAttackData.DamageDealingComponentSocketName );
	DamageDealingSphereComponent->SetDealtDamage( ActiveAttackData.DamageDealt );
	DamageDealingSphereComponent->RegisterComponent();
	DamageDealingSphereComponent->SetRelativeLocation( FVector::ZeroVector );
	DamageDealingSphereComponent->SetSphereRadius( CombatSettings.LoadSynchronous()->GetDamageDealingSphereRadius(), true );
	DamageDealingSphereComponent->SetHiddenInGame( !CombatSettings.LoadSynchronous()->ShouldShowDamageDealingSphere() );
}

void UCombatComponent::RemoveSpawnedDamageDealingSphere()
{
	UDamageDealingSphereComponent* OwnerDamageDealingSphereComponent = GetOwner()->GetComponentByClass<UDamageDealingSphereComponent>();
	if ( IsValid( OwnerDamageDealingSphereComponent ) )
	{
		OwnerDamageDealingSphereComponent->DestroyComponent();
	}
}

bool UCombatComponent::CanSoftLockOnOpponent() const
{
	if ( !IsValid( Opponent ) )
	{
		return false;
	}

	{
		if ( !IsValid( CombatSettings.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "CombatSettings is not valid" );
			return false;
		}
		if ( !IsValid( GetOwner() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "GetOwner() is not valid" );
			return false;
		}
	}

	float DistanceToOpponent = FVector::Distance( GetOwner()->GetActorLocation(), Opponent->GetActorLocation() );
	return DistanceToOpponent <= CombatSettings.LoadSynchronous()->GetSoftLockDistance();
}

void UCombatComponent::SoftLockOnOpponent()
{
	{
		if ( !IsValid( Opponent ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "Opponent is not valid" );
			return;
		}
		if ( !IsValid( GetOwner() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "Owner is not valid" );
			return;
		}
	}

	FRotator NewOwnerRotation = GetOwner()->GetActorRotation();
	NewOwnerRotation.Yaw = UKismetMathLibrary::FindLookAtRotation( GetOwner()->GetActorLocation(), Opponent->GetActorLocation() ).Yaw;
	// TODO: Make rotation speed adjustable
	GetOwner()->SetActorRotation( NewOwnerRotation );

	UpdateOwnerControlRotationAfterSoftLock();
}

void UCombatComponent::UpdateOwnerControlRotationAfterSoftLock()
{
	{
		if ( !IsValid( GetOwner() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "Owner is not valid" );
			return;
		}
	}

	float NewOwnerControlRotationYaw = GetOwner()->GetActorRotation().Yaw;

	APawn* OwnerPawn = Cast<APawn>( GetOwner() );

	{
		if ( !IsValid( OwnerPawn ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerPawn is not valid" );
			return;
		}

		if ( !IsValid( OwnerPawn->Controller ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerPawn->Controller is not valid" );
			return;
		}
	}

	FRotator NewOwnerControlRotation = OwnerPawn->GetControlRotation();
	NewOwnerControlRotation.Yaw = NewOwnerControlRotationYaw;
	OwnerPawn->Controller->SetControlRotation( NewOwnerControlRotation );
}

void UCombatComponent::RestoreStamina()
{
	CurrentStamina += StaminaRestoreAmount;

	if ( CombatSettings.LoadSynchronous()->GetMaxStamina() <= CurrentStamina )
	{
		CurrentStamina = CombatSettings.LoadSynchronous()->GetMaxStamina();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer( StaminaRestoreTimerHandle,
	                                        this,
	                                        &UCombatComponent::RestoreStamina,
	                                        CombatSettings.LoadSynchronous()->GetStaminaRestoreRate() );
}

void UCombatComponent::KnockOutOwner()
{
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	{
		if ( !IsValid( OwnerAnimInstance ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), "OwnerAnimInstance is not valid" );
			return;
		}
	}

	CharacterCombatState = ECharacterCombatState::KnockedOut;

	OwnerAnimInstance->Montage_Play( CombatSettings.LoadSynchronous()->GetKnockoutAnimMontage().LoadSynchronous() );
	OnOwnerKnockedOut.Broadcast();

	GetOwner()->OnTakeAnyDamage.RemoveDynamic( this, &UCombatComponent::OnOwnerTakeDamage );
	SetComponentTickEnabled( false );
}

bool UCombatComponent::TryBlockAttack(const float AttackDamage)
{
	//Reducing the stamina for the damage taken
	CurrentStamina -= AttackDamage;

	// Prevent stamina from being restored for a time after blocking an attack
	if ( StaminaRestoreTimerHandle.IsValid() )
	{
		StaminaRestoreTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer( StaminaRestoreTimerHandle,
	                                        this,
	                                        &UCombatComponent::RestoreStamina,
	                                        CombatSettings.LoadSynchronous()->GetStaminaAfterBlockRestoreDelay() );

	// Stamina cannot be less than 0
	CurrentStamina = FMath::Max( 0.f, CurrentStamina );

	// If stamina is 0, it means we failed to block the attack
	return 0.f < CurrentStamina;
}
