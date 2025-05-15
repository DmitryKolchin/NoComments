// Fill out your copyright notice in the Description page of Project Settings.

#include "PreCharacterMovementComponentTickPrerequisiteComponent.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NoComments/Characters/NCCharacter_Base.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

// Sets default values for this component's properties
UPreCharacterMovementComponentTickPrerequisiteComponent::UPreCharacterMovementComponentTickPrerequisiteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetTickGroup( TG_DuringPhysics );

	// ...
}

// Called when the game starts
void UPreCharacterMovementComponentTickPrerequisiteComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UPreCharacterMovementComponentTickPrerequisiteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	UpdateOwnerCharacterMovement();

	// ...
}

void UPreCharacterMovementComponentTickPrerequisiteComponent::UpdateOwnerCharacterMovement()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>( GetOwner() );

	if ( !IsValid( OwnerCharacter ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "PreCharacterMovementComponentTickPrerequisiteComponent must be attached to a character" ) );
		return;
	}

	UCharacterMovementComponent* OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();

	if ( !IsValid( OwnerCharacterMovement ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Owner character must have a character movement component" ) );
		return;
	}

	FVector PendingMovementInputVector = OwnerCharacterMovement->GetPendingInputVector();
	OwnerCharacterMovement->BrakingDecelerationWalking = PendingMovementInputVector.IsNearlyZero() ? 500.f : 2000.f;

	ANCCharacter_Base* OwnerNCCharacterBase = Cast<ANCCharacter_Base>( OwnerCharacter );

	if ( !IsValid( OwnerNCCharacterBase ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Owner character must be of type ANCCharacter_Base" ) );
		return;
	}

	FVector MovementSpeeds = FVector::ZeroVector;
	switch ( OwnerNCCharacterBase->GetMotionMatchingGait() )
	{
		case EMotionMatchingGait::Walking:
			MovementSpeeds = WalkSpeeds;
			break;
		case EMotionMatchingGait::Running:
			MovementSpeeds = RunSpeeds;
			break;
		case EMotionMatchingGait::None:
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "Owner character must have a valid motion matching gait" ) );
			return;
	}
	OwnerCharacterMovement->MaxWalkSpeed = CalculateMaxSpeedFromDirection( OwnerCharacterMovement, MovementSpeeds );
	OwnerCharacterMovement->MaxWalkSpeedCrouched = CalculateMaxSpeedFromDirection( OwnerCharacterMovement, CrouchSpeeds );
}

double UPreCharacterMovementComponentTickPrerequisiteComponent::CalculateMaxSpeedFromDirection(UCharacterMovementComponent* OwnerCharacterMovement, const FVector& Speeds) const
{
	{
		if ( !IsValid( OwnerCharacterMovement ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "OwnerCharacterMovement is not valid!" ) );
			return 0.f;
		}
		if ( !IsValid( StrafeSpeedMapCurve.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "StrafeSpeedMapCurve is not valid!" ) );
			return 0.f;
		}
	}

	float StrafeSpeedMap = 0.f;
	if ( OwnerCharacterMovement->bUseControllerDesiredRotation )
	{
		if ( !IsValid( StrafeSpeedMapCurve.LoadSynchronous() ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "StrafeSpeedMapCurve is not valid!" ) );
			return 0.f;
		}

		const FVector CharacterVelocity = OwnerCharacterMovement->Velocity;
		const float CharacterMovementDirection = UKismetAnimationLibrary::CalculateDirection( CharacterVelocity, GetOwner()->GetActorRotation() );

		StrafeSpeedMap = StrafeSpeedMapCurve.LoadSynchronous()->GetFloatValue( FMath::Abs( CharacterMovementDirection ) );
	}

	if ( StrafeSpeedMap < 1.f )
	{
		return UKismetMathLibrary::MapRangeClamped( StrafeSpeedMap, 0.f, 1.f, Speeds.X, Speeds.Y );
	}

	return UKismetMathLibrary::MapRangeClamped( StrafeSpeedMap, 1.f, 2.f, Speeds.Y, Speeds.Z );
}
