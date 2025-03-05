// Fill out your copyright notice in the Description page of Project Settings.

#include "NCCharacter_Base.h"

#include "GroomBlueprintLibrary.h"
#include "Components/LODSyncComponent.h"
#include "Components/MetahumanBuilderComponent.h"
#include "NoComments/Components/NCMetaHumanComponent.h"
#include "Settings/MetahumanComponentDataExtractorSettings.h"

DEFINE_LOG_CATEGORY( LogNCCharacter_Base );

ANCCharacter_Base::ANCCharacter_Base()
{
	MetahumanBuilderComponent = CreateDefaultSubobject<UMetahumanBuilderComponent>( TEXT( "MetahumanBuilderComponent" ) );
	MetaHumanComponent = CreateDefaultSubobject<UNCMetaHumanComponent>( TEXT( "MetaHumanComponent" ) );

	LODSyncComponent = CreateDefaultSubobject<ULODSyncComponent>( TEXT( "LODSyncComponent" ) );

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

void ANCCharacter_Base::PostActorCreated()
{
	Super::PostActorCreated();

	// No need to do anything if the object is a class default object
	// This should not be called on CDO, but just in case
	if (HasAnyFlags( RF_ClassDefaultObject ))
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

void ANCCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	SetupHairLOD();
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

USkeletalMeshComponent* ANCCharacter_Base::GetFaceSkeletalMeshComponent() const
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();
	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT("!IsValid( MetahumanComponentDataExtractorSettings )") );
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
