// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MetahumanBuilderComponent.h"

#include "GroomComponent.h"
#include "DataAssets/MetahumanBuilderComponentImportSettingsDataAsset.h"
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

	// No need to do anything if the object is a class default object
	if (HasAnyFlags( RF_ClassDefaultObject ))
	{
		return;
	}

	if (!IsValid( GetOwner() ) || GetOwner()->HasAnyFlags( RF_ClassDefaultObject ))
	{
		return;
	}

	if ( PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED( UMetahumanBuilderComponent, MetahumanComponentsDataAsset ) )
	{
		InitializeManagedOwnerComponents();
	}
}

void UMetahumanBuilderComponent::InitializeManagedOwnerComponents()
{
	// Clearing all the spawned stuff if the data asset is not valid (for example, if it was cleared)
	if ( !IsValid( MetahumanComponentsDataAsset.LoadSynchronous() ) )
	{
		DestroyManagedOwnerComponents();
		return;
	}

	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	// First things first - we need to get the owner body component
	InitializeOwnerBodyComponent();
	USceneComponent* BodyComponent = ManagedOwnerComponents.FindRef( MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName() );

	{
		if ( !IsValid( BodyComponent ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponents: BodyComponent is not valid." ) );
			return;
		}
	}

	//Let's add all the rest of skeletal mesh components to the owner
	TArray<FName> SkeletalMeshComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetSkeletalMeshComponentPropertyNames();
	SkeletalMeshComponentPropertyNames.Remove( MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName() );
	InitializeManagedOwnerComponentsFromNamesAndClass( BodyComponent, SkeletalMeshComponentPropertyNames, USkeletalMeshComponent::StaticClass() );

	//Let's add all the rest of groom  components to the owner
	USceneComponent* FaceComponent = ManagedOwnerComponents.FindRef( MetahumanComponentDataExtractorSettings->GetFaceSkeletalMeshComponentPropertyName() );

	if ( !IsValid( FaceComponent ) )
	{
		ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponents: Face component is not valid." ) );
		return;
	}

	TArray<FName> GroomComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetGroomComponentPropertyNames();
	InitializeManagedOwnerComponentsFromNamesAndClass( FaceComponent, GroomComponentPropertyNames, UGroomComponent::StaticClass() );
}

void UMetahumanBuilderComponent::InitializeOwnerBodyComponent()
{
	{
		if ( !IsValid( GetOwner() ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeOwnerBodyComponent: Owner is not valid." ) );
			return;
		}
		if ( !IsValid( MetahumanComponentsDataAsset.LoadSynchronous() ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeOwnerBodyComponent: MetahumanComponentsDataAsset is not valid." ) );
			return;
		}
		if (!IsValid( MetahumanBuilderComponentImportSettingsDataAsset ))
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass: MetahumanBuilderComponentImportSettingsDataAsset is not valid." ) );
			return;
		}
	}

	USceneComponent* OwnerBodyComponent = GetOrCreateOwnerBodyComponent();


	{
		if ( !IsValid( OwnerBodyComponent ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeOwnerBodyComponent: OwnerBodyComponent is not valid." ) );
			return;
		}
	}

	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	//Adding the body component to the map if it wasn't there
	if (!ManagedOwnerComponents.Contains( MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName() ))
	{
		ManagedOwnerComponents.Add( MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName(), OwnerBodyComponent );
	}

	FTransform BodyComponentTransform = OwnerBodyComponent->GetRelativeTransform();

	// Initialize the body component with the data from the data asset
	// Taking into account the properties to ignore
	const FName BodySkeletalMeshComponentPropertyName = MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName();
	USkeletalMeshComponent* BodySkeletalMeshComponent = MetahumanComponentsDataAsset.LoadSynchronous()->GetSkeletalMeshComponentByName( BodySkeletalMeshComponentPropertyName );
	FComponentDataImportSettings BodyDataImportSettings = MetahumanBuilderComponentImportSettingsDataAsset->GetComponentDataImportSettingsForComponentName( BodySkeletalMeshComponentPropertyName );

	UBlueprintDataExtractionEFL::CopyPropertiesFromOneObjectToAnother( BodySkeletalMeshComponent, OwnerBodyComponent, BodyDataImportSettings.PropertiesToNotCopy );

	OwnerBodyComponent->UnregisterComponent();
	OwnerBodyComponent->RegisterComponent();
	OwnerBodyComponent->SetRelativeTransform( BodyComponentTransform );
}

void UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass(USceneComponent* ComponentToAttachTo, const TArray<FName>& ComponentNames, const TSubclassOf<USceneComponent>& ComponentClass)
{
	{
		if ( !IsValid( MetahumanComponentsDataAsset.LoadSynchronous() ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass: MetahumanComponentsDataAsset is not valid." ) );
			return;
		}
		if ( !IsValid( ComponentToAttachTo ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass: ComponentToAttachTo is not valid." ) );
			return;
		}

		if (!IsValid( MetahumanBuilderComponentImportSettingsDataAsset ))
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass: MetahumanBuilderComponentImportSettingsDataAsset is not valid." ) );
			return;
		}
	}

	for ( auto ComponentName : ComponentNames )
	{
		USceneComponent* ComponentToTakePropertiesFrom = MetahumanComponentsDataAsset.LoadSynchronous()->GetSceneComponentByName( ComponentName );

		{
			if ( !IsValid( ComponentToTakePropertiesFrom ) )
			{
				ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass: ComponentToTakePropertiesFrom is not valid." ) );
				return;
			}
		}

		USceneComponent* CurrentProcessedComponent = ManagedOwnerComponents.Contains( ComponentName )
			                                             ? ManagedOwnerComponents.FindRef( ComponentName )
			                                             : AddSceneComponentToOwner( ComponentName, ComponentToAttachTo, ComponentClass );

		{
			if ( !IsValid( CurrentProcessedComponent ) )
			{
				ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::InitializeManagedOwnerComponentsFromNamesAndClass: CurrentProcessedComponent is not valid." ) );
				return;
			}
		}

		FComponentDataImportSettings ComponentDataImportSettings = MetahumanBuilderComponentImportSettingsDataAsset->GetComponentDataImportSettingsForComponentName( ComponentName );
		UBlueprintDataExtractionEFL::CopyPropertiesFromOneObjectToAnother( ComponentToTakePropertiesFrom,
		                                                                   CurrentProcessedComponent,
		                                                                   ComponentDataImportSettings.PropertiesToNotCopy );
		CurrentProcessedComponent->UnregisterComponent();
		CurrentProcessedComponent->RegisterComponent();

		//Adding new component to map if it wasn't there
		if ( !ManagedOwnerComponents.Contains( ComponentName ) )
		{
			ManagedOwnerComponents.Add( ComponentName, CurrentProcessedComponent );
		}
	}
}

USkeletalMeshComponent* UMetahumanBuilderComponent::GetOrCreateOwnerBodyComponent()
{
	{
		if ( !IsValid( GetOwner() ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::GetOwnerBodyComponent: Owner is not valid." ) );
			return nullptr;
		}
	}

	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();

	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent: MetahumanComponentDataExtractorSettings is not valid." ) );
			return nullptr;
		}
	}

	FName BodySkeletalMeshComponentPropertyName = MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName();

	if ( ManagedOwnerComponents.Contains( BodySkeletalMeshComponentPropertyName ) )
	{
		return Cast<USkeletalMeshComponent>( ManagedOwnerComponents[ MetahumanComponentDataExtractorSettings->GetBodySkeletalMeshComponentPropertyName() ] );
	}

	TSet<UActorComponent*> OwnerComponents = GetOwner()->GetComponents();

	// By default, the skeletal mesh which parent is not a skeletal mesh is the body
	// If the body name override is provided, we use it
	for ( auto OwnerComponent : OwnerComponents )
	{
		USkeletalMeshComponent* OwnerSkeletalMeshComponent = Cast<USkeletalMeshComponent>( OwnerComponent );

		// We are not interested in non-skeletal mesh components
		if ( !IsValid( OwnerSkeletalMeshComponent ) )
		{
			continue;
		}

		// Check if the body name override is provided
		if ( !BodySkeletalMeshOverrideName.IsEqual( NAME_None ) )
		{
			if ( OwnerSkeletalMeshComponent->GetFName().IsEqual( BodySkeletalMeshOverrideName ) )
			{
				return OwnerSkeletalMeshComponent;
			}

			continue;
		}

		// Otherwise we use the first skeletal mesh component which parent is not a skeletal mesh
		USkeletalMeshComponent* ParentSkeletalMeshComponent = Cast<USkeletalMeshComponent>( OwnerSkeletalMeshComponent->GetAttachParent() );
		if ( !IsValid( ParentSkeletalMeshComponent ) )
		{
			return OwnerSkeletalMeshComponent;
		}
	}

	//if we didn't find the body component, we shall create one
	return Cast<USkeletalMeshComponent>( AddSceneComponentToOwner( BodySkeletalMeshComponentPropertyName,
	                                                               GetOwner()->GetRootComponent(),
	                                                               USkeletalMeshComponent::StaticClass() ) );
}

USceneComponent* UMetahumanBuilderComponent::AddSceneComponentToOwner(FName ComponentName, USceneComponent* ParentComponent, const TSubclassOf<USceneComponent>& ComponentClass)
{
	if ( !IsValid( GetOwner() ) )
	{
		ensureAlwaysMsgf( false, TEXT( "UMetahumanBuilderComponent::AddSceneComponentToOwner: Owner is not valid." ) );
		return nullptr;
	}

	USceneComponent* NewSceneComponent = NewObject<USceneComponent>( GetOwner(), ComponentClass, ComponentName );
	NewSceneComponent->AttachToComponent( ParentComponent, FAttachmentTransformRules::KeepRelativeTransform );
	NewSceneComponent->RegisterComponent();
	GetOwner()->AddInstanceComponent( NewSceneComponent );

	return NewSceneComponent;
}

void UMetahumanBuilderComponent::DestroyManagedOwnerComponents()
{
	FName BodySkeletalMeshComponentPropertyName = GetDefault<UMetahumanComponentDataExtractorSettings>()->GetBodySkeletalMeshComponentPropertyName();
	for ( auto [ ComponentName, Component ] : ManagedOwnerComponents )
	{
		if ( ComponentName == BodySkeletalMeshComponentPropertyName )
		{
			continue;
		}

		if ( !IsValid( Component ) )
		{
			continue;
		}
		Component->DestroyComponent();
	}
}
