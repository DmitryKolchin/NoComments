// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/MetahumanComponentsDataAsset.h"

#include "GroomComponent.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Libraries/BlueprintDataExtractionEFL.h"
#include "Settings/MetahumanComponentDataExtractorSettings.h"

UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset()
{
	if (!HasAnyFlags( RF_ClassDefaultObject ))
	{
		return;
	}

	const UMetahumanComponentDataExtractorSettings* MetahumanComponentDataExtractorSettings = GetDefault<UMetahumanComponentDataExtractorSettings>();
	{
		if ( !IsValid( MetahumanComponentDataExtractorSettings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::UMetahumanComponentsDataAsset: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	TArray<FName> SkeletalMeshComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetSkeletalMeshComponentPropertyNames();

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

	EmbedSourceMetahumanBlueprintThumbnail( NewMetahumanBlueprint );
	SourceMetahumanBlueprint = NewMetahumanBlueprint;

	TArray<FName> SkeletalMeshComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetSkeletalMeshComponentPropertyNames();
	TArray<FName> GroomComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetGroomComponentPropertyNames();

	TArray<UActorComponent*> ActorComponents = UBlueprintDataExtractionEFL::ExtractAllBlueprintCreatedComponents( SourceMetahumanBlueprint );

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

UBlueprint* UMetahumanComponentsDataAsset::GetSourceMetahumanBlueprint() const
{
	return SourceMetahumanBlueprint;
}

void UMetahumanComponentsDataAsset::EmbedSourceMetahumanBlueprintThumbnail(UBlueprint* MetahumanBlueprint)
{
	{
		if ( !IsValid( MetahumanBlueprint ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMetahumanComponentsDataAsset::EmbedSourceMetahumanBlueprintThumbnail: SourceMetahumanBlueprint is not valid." ) );
			return;
		}
	}

	// First things first - getting the thumbnail from metahuman blueprint
	FObjectThumbnail* SourceMetahumanBlueprintThumbnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk( MetahumanBlueprint );

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

	ThumbnailTools::CacheEmptyThumbnail( GetFullName(), DataAssetPackage );
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

	// Signalling that the package is dirty and needs to be saved (by the god)
	DataAssetPackage->MarkPackageDirty();

	FAssetRegistryModule::AssetCreated( this ); // ✅ Forces UI refresh
}
