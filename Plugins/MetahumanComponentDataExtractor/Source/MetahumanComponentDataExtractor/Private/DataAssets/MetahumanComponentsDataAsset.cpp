// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/MetahumanComponentsDataAsset.h"

#include "GroomComponent.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Libraries/BlueprintDataExtractionEFL.h"
#include "Settings/MetahumanComponentDataExtractorSettings.h"

UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset()
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();
	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	TArray<FName> SkeletalMeshComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetSkeletalMeshComponentPropertyNames();

	// It's easier to manipulate the data asset if the components are stored in separate maps
	for ( auto SkeletalMeshComponentPropertyName : SkeletalMeshComponentPropertyNames )
	{
		USkeletalMeshComponent* NewSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>( SkeletalMeshComponentPropertyName );

		SkeletalMeshComponents.Add( SkeletalMeshComponentPropertyName, NewSkeletalMeshComponent );
	}

	TArray<FName> GroomComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetGroomComponentPropertyNames();
	for ( auto GroomComponentPropertyName : GroomComponentPropertyNames )
	{
		UGroomComponent* NewGroomComponent = CreateDefaultSubobject<UGroomComponent>( GroomComponentPropertyName );

		GroomComponents.Add( GroomComponentPropertyName, NewGroomComponent );
	}
}

void UMetahumanComponentsDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty( PropertyChangedEvent );

	if ( PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED( UMetahumanComponentsDataAsset, SourceMetahumanBlueprint ) )
	{
		UpdateEmbeddedThumbnail();
	}
}

void UMetahumanComponentsDataAsset::ExtractDataFromMetahumanBlueprint(UObject* Object)
{
	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();
	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}
	UBlueprint* NewMetahumanBlueprint = Cast<UBlueprint>( Object );
	if ( !IsValid( NewMetahumanBlueprint ) )
	{
		ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: Object is not a blueprint." ) );
		return;
	}

	//Cache the source metahuman blueprint
	SourceMetahumanBlueprint = NewMetahumanBlueprint;

	TArray<FName> SkeletalMeshComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetSkeletalMeshComponentPropertyNames();
	TArray<FName> GroomComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetGroomComponentPropertyNames();

	TArray<UActorComponent*> ActorComponents = UBlueprintDataExtractionEFL::ExtractAllBlueprintCreatedComponents( SourceMetahumanBlueprint.LoadSynchronous() );

	for ( auto ActorComponent : ActorComponents )
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>( ActorComponent );
		UGroomComponent* GroomComponent = Cast<UGroomComponent>( ActorComponent );

		FName ComponentName = ActorComponent->GetFName();
		FString ComponentNameString = ComponentName.ToString();

		for ( auto SkeletalMeshComponentPropertyName : SkeletalMeshComponentPropertyNames )
		{
			if ( ComponentNameString.StartsWith( SkeletalMeshComponentPropertyName.ToString() ) )
			{
				UBlueprintDataExtractionEFL::CopyAllPropertiesFromOneObjectToAnother( SkeletalMeshComponent, SkeletalMeshComponents[ SkeletalMeshComponentPropertyName ] );
			}
		}

		for ( auto GroomComponentPropertyName : GroomComponentPropertyNames )
		{
			if ( ComponentNameString.StartsWith( GroomComponentPropertyName.ToString() ) )
			{
				UBlueprintDataExtractionEFL::CopyAllPropertiesFromOneObjectToAnother( GroomComponent, GroomComponents[ GroomComponentPropertyName ] );
			}
		}
	}
}

USkeletalMeshComponent* UMetahumanComponentsDataAsset::GetSkeletalMeshComponentByName(FName ComponentName) const
{
	if ( SkeletalMeshComponents.Contains( ComponentName ) )
	{
		return SkeletalMeshComponents[ ComponentName ];
	}

	return nullptr;
}

UGroomComponent* UMetahumanComponentsDataAsset::GetGroomComponentByName(FName ComponentName) const
{
	if ( GroomComponents.Contains( ComponentName ) )
	{
		return GroomComponents[ ComponentName ];
	}

	return nullptr;
}

bool UMetahumanComponentsDataAsset::HasSourceMetahumanBlueprint() const
{
	return IsValid( SourceMetahumanBlueprint.LoadSynchronous() );
}

void UMetahumanComponentsDataAsset::UpdateEmbeddedThumbnail()
{
	// First of all - clearing the old cached thumbnail
	ThumbnailTools::CacheEmptyThumbnail( GetFullName(), GetPackage() );

	// This can be if the source metahuman blueprint is not set or cleared
	if ( !HasSourceMetahumanBlueprint() )
	{
		return;
	}

	// First things first - getting the thumbnail from metahuman blueprint
	FObjectThumbnail* SourceMetahumanBlueprintThumbnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk( SourceMetahumanBlueprint.LoadSynchronous() );

	{
		if ( !SourceMetahumanBlueprintThumbnail )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::EmbedSourceMetahumanBlueprintThumbnail: SourceMetahumanBlueprintThumbnail is not valid." ) );
			return;
		}
	}

	FObjectThumbnail ThumbnailCopy = *SourceMetahumanBlueprintThumbnail;

	// Now let's embed the thumbnail into the data asset blueprint by adding it to the thumbnail map
	UPackage* DataAssetPackage = GetPackage();
	FString DataAssetPackageFullName = DataAssetPackage->GetPathName();
	FName DataAssetPackageFullNameAsFName = FName( *DataAssetPackageFullName );

	// if there is a thumbnail map, we can set the thumbnail we took from the metahuman blueprint as the thumbnail for the data asset
	if ( DataAssetPackage->HasThumbnailMap() )
	{
		FThumbnailMap& DataAssetThumbnailMap = DataAssetPackage->AccessThumbnailMap();
		DataAssetThumbnailMap.Add( DataAssetPackageFullNameAsFName, ThumbnailCopy );
	}
	// if there is no thumbnail map, we need to create one
	else
	{
		FThumbnailMap DataAssetThumbnailMap;
		DataAssetThumbnailMap.Add( DataAssetPackageFullNameAsFName, ThumbnailCopy );
		DataAssetPackage->SetThumbnailMap( MakeUnique<FThumbnailMap>( DataAssetThumbnailMap ) );
	}


	FAssetRegistryModule::AssetCreated( this );
}
