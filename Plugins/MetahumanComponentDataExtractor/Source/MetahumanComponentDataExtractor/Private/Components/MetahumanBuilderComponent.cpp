// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MetahumanBuilderComponent.h"

#include "GroomComponent.h"
#include "DataAssets/MetahumanComponentsDataAsset.h"
#include "Libraries/BlueprintDataExtractionEFL.h"
#include "Settings/MetahumanComponentDataExtractorSettings.h"

DEFINE_LOG_CATEGORY_STATIC( LogMetahumanBuilderComponent, Log, All );

class UMetahumanComponentDataExtractorSettings;
// Sets default values for this component's properties
UMetahumanBuilderComponent::UMetahumanBuilderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMetahumanBuilderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UMetahumanBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UMetahumanBuilderComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty( PropertyChangedEvent );

	if ( PropertyChangedEvent.Property->GetName() == FName( "MetahumanComponentsDataAsset" ).ToString() )
	{
		DestroySpawnedComponents();

		if ( !IsValid( GetOwner() ) )
		{
			return;
		}

		USkeletalMeshComponent* Body = nullptr;
		for ( auto Component : GetOwner()->GetComponents() )
		{
			if ( Component->GetName().StartsWith( OwnerBodyComponentName.ToString() ) )
			{
				Body = Cast<USkeletalMeshComponent>( Component );
				break;
			}
		}
		AddComponentsToOwner( Body );
	}
}

void UMetahumanBuilderComponent::AddComponentsToOwner(USkeletalMeshComponent* Body)
{
	{
		if ( !IsValid( Body ) )
		{
			UE_LOG( LogMetahumanBuilderComponent, Error, TEXT( "UMetahumanBuilderComponent::AddComponentsToOwner: Body is not valid." ) );
			return;
		}
	}

	{
		if ( !MetahumanComponentsDataAsset.LoadSynchronous() )
		{
			UE_LOG( LogMetahumanBuilderComponent, Error, TEXT( "UMetahumanBuilderComponent::AddComponentsToOwner: MetahumanComponentsDataAsset is not valid." ) );
			return;
		}
	}

	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	DestroySpawnedComponents();

	SetupOwnerBodySkeletalMeshComponent( Body );

	AddSkeletalMeshComponentsToOwnerBody( Body );

	USkeletalMeshComponent* Face = GetOwnerFaceSkeletalMeshComponent();

	if ( !IsValid( Face ) )
	{
		UE_LOG( LogMetahumanBuilderComponent, Error, TEXT( "UMetahumanBuilderComponent::AddComponentsToOwner: Face is not valid." ) );
		return;
	}

	AddGrooomComponentsToOwnerFace( Face );
}

void UMetahumanBuilderComponent::AddNewSceneComponentToOwnerFromExistingSceneComponent(USceneComponent* ExistingComponent,
                                                                                       USceneComponent* ParentComponent,
                                                                                       FName ComponentPropertyName,
                                                                                       const TSubclassOf<USceneComponent>& ComponentClass)
{
	USceneComponent* NewSceneComponent = NewObject<USceneComponent>( GetOwner(), ComponentClass, ComponentPropertyName );
	UBlueprintDataExtractionEFL::CopyAllPropertiesFromOneObjectToAnother( ExistingComponent, NewSceneComponent );
	NewSceneComponent->AttachToComponent( ParentComponent, FAttachmentTransformRules::KeepRelativeTransform );
	NewSceneComponent->RegisterComponent();
	GetOwner()->AddInstanceComponent( NewSceneComponent );

	SpawnedComponents.Add( NewSceneComponent );
}

void UMetahumanBuilderComponent::SetupOwnerBodySkeletalMeshComponent(USkeletalMeshComponent* Body)
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	const FName DefaultBodyComponentName = MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName();
	OwnerBodyComponentName = Body->GetFName();

	FTransform BodyTransform = Body->GetComponentTransform();
	UBlueprintDataExtractionEFL::CopyAllPropertiesFromOneObjectToAnother( MetahumanComponentsDataAsset.LoadSynchronous()->GetSkeletalMeshComponentByName( DefaultBodyComponentName ), Body );


	Body->UnregisterComponent();
	Body->RegisterComponent();
	Body->SetRelativeTransform( BodyTransform );
}

void UMetahumanBuilderComponent::AddSkeletalMeshComponentsToOwnerBody(USkeletalMeshComponent* Body)
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	const FName DefaultBodyComponentName = MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName();
	TArray<FName> SkeletalMeshComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetSkeletalMeshComponentPropertyNames();
	SkeletalMeshComponentPropertyNames.Remove( DefaultBodyComponentName );

	for ( auto SkeletalMeshComponentPropertyName : SkeletalMeshComponentPropertyNames )
	{
		USkeletalMeshComponent* SkeletalMeshComponentFromDataAsset = MetahumanComponentsDataAsset.LoadSynchronous()->GetSkeletalMeshComponentByName( SkeletalMeshComponentPropertyName );
		AddNewSceneComponentToOwnerFromExistingSceneComponent( SkeletalMeshComponentFromDataAsset, Body, SkeletalMeshComponentPropertyName, USkeletalMeshComponent::StaticClass() );
	}
}

void UMetahumanBuilderComponent::AddGrooomComponentsToOwnerFace(USkeletalMeshComponent* Face)
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	TArray<FName> GroomComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetGroomComponentPropertyNames();

	for ( auto GroomComponentPropertyName : GroomComponentPropertyNames )
	{
		UGroomComponent* GroomComponentFromDataAsset = MetahumanComponentsDataAsset.LoadSynchronous()->GetGroomComponentByName( GroomComponentPropertyName );
		AddNewSceneComponentToOwnerFromExistingSceneComponent( GroomComponentFromDataAsset, Face, GroomComponentPropertyName, UGroomComponent::StaticClass() );
	}
}

USkeletalMeshComponent* UMetahumanBuilderComponent::GetOwnerFaceSkeletalMeshComponent() const
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return nullptr;
		}
	}

	for ( auto Component : GetOwner()->GetComponents() )
	{
		if ( Component->GetName().StartsWith( "Face" ) )
		{
			return Cast<USkeletalMeshComponent>( Component );
		}
	}

	return nullptr;
}

void UMetahumanBuilderComponent::DestroySpawnedComponents()
{
	for ( auto SpawnedComponent : SpawnedComponents )
	{
		if ( IsValid( SpawnedComponent ) )
		{
			SpawnedComponent->DestroyComponent();
		}
	}

	SpawnedComponents.Empty();
}
