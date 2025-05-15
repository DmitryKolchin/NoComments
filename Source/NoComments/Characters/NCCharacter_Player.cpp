// Fill out your copyright notice in the Description page of Project Settings.

#include "NCCharacter_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NoComments/Components/CombatComponent.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"

ANCCharacter_Player::ANCCharacter_Player()
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>( TEXT( "CombatComponent" ) );

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComponent" ) );
	SpringArmComponent->SetupAttachment( GetRootComponent() );

	CameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComponent" ) );
	CameraComponent->SetupAttachment( SpringArmComponent );

}

void ANCCharacter_Player::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	return;
}

FGenericTeamId ANCCharacter_Player::GetGenericTeamId() const
{
	return TeamId;
}

void ANCCharacter_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy( NewController );

	// Adding the default input mapping context whenever the player is possessed by a player controller
	APlayerController* PlayerController = Cast<APlayerController>( NewController );
	if (IsValid( PlayerController ))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() );
		if ( !IsValid( EnhancedInputLocalPlayerSubsystem ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "EnhancedInputLocalPlayerSubsystem is not valid!" ) );
			return;
		}

		EnhancedInputLocalPlayerSubsystem->AddMappingContext( DefaultInputMappingContext, 0 );
	}

	if ( IGenericTeamAgentInterface* ControllerAsTeamAgent = Cast<IGenericTeamAgentInterface>( NewController ) )
	{
		TeamId = ControllerAsTeamAgent->GetGenericTeamId();
	}
}

void ANCCharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>( PlayerInputComponent );

	{
		if ( !IsValid( EnhancedInputComponent ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "EnhancedInputComponent is not valid!" ) );
			return;
		}

		if ( !DefaultInputMappingContext )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "DefaultInputMappingContext is not valid!" ) );
			return;
		}
	}

	EnhancedInputComponent->BindAction( MoveInputAction, ETriggerEvent::Triggered, this, &ANCCharacter_Player::Move );
	EnhancedInputComponent->BindAction( RotateCameraInputAction, ETriggerEvent::Triggered, this, &ANCCharacter_Player::RotateCamera );
}

void ANCCharacter_Player::Tick(float DeltaSeconds)
{
	Super::Tick( DeltaSeconds );

	// Camera updates
	if ( !IsValid( CameraComponent ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "CameraComponent is not valid!" ) );
		return;
	}

	FRotator ControlRotation = GetControlRotation();

	FRotator SpringArmComponentRotation = ControlRotation;
	SpringArmComponentRotation.Pitch += CurrentCameraPitchOffset;
	SpringArmComponentRotation.Yaw += CurrentCameraYawOffset;
	SpringArmComponentRotation.Roll = 0.f;

	SpringArmComponent->SetWorldRotation( SpringArmComponentRotation );

	if ( GetActiveCameraSettings().bIncreaseFOVWhenMoving )
	{
		if ( !GetVelocity().IsNearlyZero() )
		{
			SetFOV_Movement();
		}
		else
		{
			SetFOV_Default();
		}
	}
}

void ANCCharacter_Player::BeginPlay()
{
	Super::BeginPlay();

	if ( !IsValid( CameraSettingsDataAsset ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "CameraSettingsDataAsset is not valid!" ) );
		return;
	}
	ResetCameraSettingsToDefault();

}

UCombatComponent* ANCCharacter_Player::GetCombatComponent() const
{
	return CombatComponent;
}

FCameraSettings ANCCharacter_Player::GetActiveCameraSettings() const
{
	return ActiveCameraSettings;
}

void ANCCharacter_Player::SetFightModeCameraSettings()
{
	if ( !IsValid( CameraSettingsDataAsset ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "CameraSettingsDataAsset is not valid!" ) );
		return;
	}
	CurrentCameraPitchOffset = 0.f;
	CurrentCameraYawOffset = 0.f;

	ActiveCameraSettings = CameraSettingsDataAsset->GetFightModeCameraSettings();
}

void ANCCharacter_Player::ResetCameraSettingsToDefault()
{
	if ( !IsValid( CameraSettingsDataAsset ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "CameraSettingsDataAsset is not valid!" ) );
		return;
	}
	ActiveCameraSettings = CameraSettingsDataAsset->GetDefaultCameraSettings();
}

void ANCCharacter_Player::Move(const FInputActionInstance& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if ( !IsValid( AnimInstance ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "AnimInstance is not valid!" ) );
		return;
	}

	// No movement if the character is playing a montage
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

	FVector InputDirection = Value.GetValue().Get<FVector>();
	InputDirection.Normalize(  );


	FVector RightDirection = CameraComponent->GetRightVector();
	FVector ForwardDirection = CameraComponent->GetForwardVector() * FVector{1.f, 1.f, 0.f};

	FVector MovementDirection = ( RightDirection * InputDirection.X + ForwardDirection * InputDirection.Y ).GetSafeNormal();
	AddMovementInput( MovementDirection );

}

void ANCCharacter_Player::RotateCamera(const FInputActionInstance& Value)
{
	FVector InputDirection = Value.GetValue().Get<FVector>();
	// Camera rotation is implemented via the intermediate rotation offsets for the spring arm component
	// It allows to rotate camera without modifying the controller rotation, and keep it attached to the mentioned control rotation at the same time

	if ( ActiveCameraSettings.YawRotationRange.HasLowerBound() && ActiveCameraSettings.YawRotationRange.HasUpperBound() )
	{
		CurrentCameraYawOffset = FMath::Clamp( InputDirection.X * CameraTurnRate + CurrentCameraYawOffset,
		                                       ActiveCameraSettings.YawRotationRange.GetLowerBound().GetValue(),
		                                       ActiveCameraSettings.YawRotationRange.GetUpperBound().GetValue() );
	}
	else
	{
		CurrentCameraYawOffset += InputDirection.X * CameraTurnRate;
	}

	if ( ActiveCameraSettings.PitchRotationRange.HasLowerBound() && ActiveCameraSettings.PitchRotationRange.HasUpperBound() )
	{
		CurrentCameraPitchOffset = FMath::Clamp( InputDirection.Y * CameraTurnRate + CurrentCameraPitchOffset,
		                                         ActiveCameraSettings.PitchRotationRange.GetLowerBound().GetValue(),
		                                         ActiveCameraSettings.PitchRotationRange.GetUpperBound().GetValue() );
	}
	else
	{
		CurrentCameraPitchOffset += InputDirection.Y * CameraTurnRate;
	}
}

void ANCCharacter_Player::Uppercut()
{
	if ( !IsValid( CombatComponent ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "CombatComponent is not valid!" ) );
		return;
	}

	if ( !CombatComponent->CanPerformGivenAttack( UppercutAttackData ) )
	{
		return;
	}

	CombatComponent->Attack( UppercutAttackData );
}

void ANCCharacter_Player::PowerPunch()
{
	if ( !IsValid( CombatComponent ) )
	{
		UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "CombatComponent is not valid!" ) );
		return;
	}
	if ( !CombatComponent->CanPerformGivenAttack( PowerPunchAttackData ) )
	{
		return;
	}
	CombatComponent->Attack( PowerPunchAttackData );
}
