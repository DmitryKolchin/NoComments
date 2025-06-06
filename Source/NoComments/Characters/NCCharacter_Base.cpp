﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "NCCharacter_Base.h"

#include "GroomBlueprintLibrary.h"
#include "KismetAnimationLibrary.h"
#include "Components/LODSyncComponent.h"
#include "Components/MetahumanBuilderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NoComments/Components/NCMetaHumanComponent.h"
#include "NoComments/Components/PreCharacterMovementComponentTickPrerequisiteComponent.h"
#include "NoComments/Utils/Libraries/DebugFunctionLibrary.h"
#include "NoComments/Utils/Macros/NC_Macro.h"
#include "Settings/MetahumanComponentDataExtractorSettings.h"

DEFINE_LOG_CATEGORY( LogNCCharacter_Base );

ANCCharacter_Base::ANCCharacter_Base()
{
	SetupMovementComponent();

	// Metahuman related stuff
	MetahumanBuilderComponent = CreateDefaultSubobject<UMetahumanBuilderComponent>( TEXT( "MetahumanBuilderComponent" ) );
	MetaHumanComponent = CreateDefaultSubobject<UNCMetaHumanComponent>( TEXT( "MetaHumanComponent" ) );

	// LODs
	LODSyncComponent = CreateDefaultSubobject<ULODSyncComponent>( TEXT( "LODSyncComponent" ) );
	SetupLODSyncComponent();

	// Motion matching
	if ( !bDisableMotionMatching )
	{
		PreCharacterMovementComponentTickPrerequisiteComponent = CreateDefaultSubobject<UPreCharacterMovementComponentTickPrerequisiteComponent>( TEXT( "PreCharacterMovementComponentTickPrerequisiteComponent" ) );
	}
}

void ANCCharacter_Base::PostActorCreated()
{
	Super::PostActorCreated();

	// No need to do anything if the object is a class default object
	// This should not be called on CDO, but just in case
	if ( HasAnyFlags( RF_ClassDefaultObject ) )
	{
		return;
	}

	if ( !IsValid( MetahumanBuilderComponent ) )
	{
		ensureAlwaysMsgf( false, TEXT( "!IsValid( MetahumanBuilderComponent )" ) );
		return;
	}

	MetahumanBuilderComponent->InitializeManagedOwnerComponents();
}

/*
void ANCCharacter_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction( Transform );

	// No need to do anything if the object is a class default object
	// This should not be called on CDO, but just in case
	if ( HasAnyFlags( RF_ClassDefaultObject ) )
	{
		return;
	}

	if ( !IsValid( MetahumanBuilderComponent ) )
	{
		ensureAlwaysMsgf( false, TEXT( "!IsValid( MetahumanBuilderComponent )" ) );
		return;
	}

	if (MetahumanBuilderComponent->GetBodySkeletalMeshOverrideName().Compare( NAME_None ))


	MetahumanBuilderComponent->InitializeManagedOwnerComponents();
}
*/
void ANCCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	SetupHairLOD();

	if ( !bDisableMotionMatching )
	{
		AddTickPrerequisiteComponent( PreCharacterMovementComponentTickPrerequisiteComponent );
	}
}

UMetahumanBuilderComponent* ANCCharacter_Base::GetMetahumanBuilderComponent() const
{
	return MetahumanBuilderComponent;
}

EMotionMatchingGait ANCCharacter_Base::GetMotionMatchingGait() const
{
	return MotionMatchingGait;
}

void ANCCharacter_Base::SetupHairLOD()
{
	if ( UGroomBlueprintLibrary::IsHairStrandsSupportedInWorld( this ) )
	{
		return;
	}

	// If strands are disabled, then make fused eyelashes visible on lower LODs
	USkeletalMeshComponent* FaceSkeletalMeshComponent = GetFaceSkeletalMeshComponent();

	{
		// If face skeletal mesh component is not valid, then log an error - we do not want to crash if the component was not created
		if ( !IsValid( FaceSkeletalMeshComponent ) )
		{
			UE_LOG( LogNCCharacter_Base, Error, TEXT( "Face skeletal mesh component is not valid" ) );
			return;
		}
	}

	UMaterialInstanceDynamic* EyelashesHiLODMaterialInstance = Cast<UMaterialInstanceDynamic>( FaceSkeletalMeshComponent->GetMaterialByName( EyelashesHiLODMaterialSlotName ) );

	{
		if ( !IsValid( EyelashesHiLODMaterialInstance ) )
		{
			ensureAlwaysMsgf( false, TEXT( "!IsValid( EyelashesHiLODMaterialInstance )" ) );
			return;
		}
	}

	UMaterialInstanceDynamic* EyelashesMaterialInstance = Cast<UMaterialInstanceDynamic>( FaceSkeletalMeshComponent->GetMaterialByName( EyelashesMaterialSlotName ) );

	{
		if ( !IsValid( EyelashesMaterialInstance ) )
		{
			ensureAlwaysMsgf( false, TEXT( "!IsValid( EyelashesMaterialInstance )" ) );
			return;
		}
	}

	//Getting the opacity value from HiLOD material
	const FName OpacityParameterName = TEXT( "Opacity" );
	const FHashedMaterialParameterInfo OpacityParameterInfo( OpacityParameterName );
	float EyelashesHiLODOpacityValue;
	EyelashesHiLODMaterialInstance->GetScalarParameterValue( OpacityParameterInfo, EyelashesHiLODOpacityValue );

	//Setting the opacity value to the eyelashes material
	EyelashesMaterialInstance->SetScalarParameterValue( OpacityParameterName, EyelashesHiLODOpacityValue );

	//Getting the opacity rt value from HiLOD material
	const FName OpacityRTParameterName = TEXT( "OpacityRT" );
	const FHashedMaterialParameterInfo OpacityRTParameterInfo( OpacityRTParameterName );
	float EyelashesHiLODOpacityRTValue;
	EyelashesHiLODMaterialInstance->GetScalarParameterValue( OpacityRTParameterInfo, EyelashesHiLODOpacityRTValue );

	//Setting the opacity rt value to the eyelashes material
	EyelashesMaterialInstance->SetScalarParameterValue( OpacityRTParameterName, EyelashesHiLODOpacityRTValue );
}

void ANCCharacter_Base::SetupLODSyncComponent()
{
	LODSyncComponent->NumLODs = 3;

	FComponentSync BodySync{FName{"Body"}, ESyncOption::Drive};
	FComponentSync FaceSync{FName{"Face"}, ESyncOption::Drive};
	FComponentSync TorsoSync{FName{"Torso"}, ESyncOption::Passive};
	FComponentSync LegsSync{FName{"Legs"}, ESyncOption::Passive};
	FComponentSync FeetSync{FName{"Feet"}, ESyncOption::Passive};
	FComponentSync HairSync{FName{"Hair"}, ESyncOption::Passive};
	FComponentSync EyebrowsSync{FName{"Eyebrows"}, ESyncOption::Passive};
	FComponentSync MustacheSync{FName{"Mustache"}, ESyncOption::Passive};
	FComponentSync BeardSync{FName{"Beard"}, ESyncOption::Passive};

	LODSyncComponent->ComponentsToSync.Add( BodySync );
	LODSyncComponent->ComponentsToSync.Add( FaceSync );
	LODSyncComponent->ComponentsToSync.Add( TorsoSync );
	LODSyncComponent->ComponentsToSync.Add( LegsSync );
	LODSyncComponent->ComponentsToSync.Add( FeetSync );
	LODSyncComponent->ComponentsToSync.Add( HairSync );
	LODSyncComponent->ComponentsToSync.Add( EyebrowsSync );
	LODSyncComponent->ComponentsToSync.Add( MustacheSync );
	LODSyncComponent->ComponentsToSync.Add( BeardSync );

	FLODMappingData GroomMappingData;
	GroomMappingData.Mapping = {3, 5, 7};
	LODSyncComponent->CustomLODMapping.Add( FName{"Hair"}, GroomMappingData );
	LODSyncComponent->CustomLODMapping.Add( FName{"Beard"}, GroomMappingData );
	LODSyncComponent->CustomLODMapping.Add( FName{"Mustache"}, GroomMappingData );
	LODSyncComponent->CustomLODMapping.Add( FName{"Eyebrows"}, GroomMappingData );
}

void ANCCharacter_Base::SetupMovementComponent()
{
	/* Setting up stuff for motion matching to work */
	GetCharacterMovement()->MaxAcceleration = 800.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;

	// Walking
	GetCharacterMovement()->GroundFriction = 5.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 150.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->PerchRadiusThreshold = 20.f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	//Rotation
	GetCharacterMovement()->RotationRate = FRotator( 0.f, -1.f, 0.f );
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	//Navigation
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

USkeletalMeshComponent* ANCCharacter_Base::GetFaceSkeletalMeshComponent() const
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();
	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			UDebugFunctionLibrary::ThrowDebugError( GET_FUNCTION_NAME_STRING(), TEXT( "!IsValid( MetahumanComponentDataExtractorSettings )" ) );
			return nullptr;
		}
	}

	const FName FaceSkeletalMeshComponentName = MetahumanComponentDataExtractorSettings->GetFaceSkeletalMeshComponentPropertyName();

	// Face skeletal mesh is guaranteed to have a name that is set in the settings
	// because it was generated by the data extractor

	TSet<UActorComponent*> ActorComponents = GetComponents();
	for ( auto Component : ActorComponents )
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>( Component );
		if ( !IsValid( SkeletalMeshComponent ) )
		{
			continue;
		}

		if ( SkeletalMeshComponent->GetFName().IsEqual( FaceSkeletalMeshComponentName ) )
		{
			return SkeletalMeshComponent;
		}
	}

	return nullptr;
}

void ANCCharacter_Base::SetMotionMatchingGait(EMotionMatchingGait NewGait)
{
	MotionMatchingGait = NewGait;
}
