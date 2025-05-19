// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/MetahumanComponentsDataAsset.h"

#include "GroomComponent.h"
#include "Libraries/BlueprintDataExtractionFL.h"
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

	TArray<UActorComponent*> ActorComponents = UBlueprintDataExtractionFL::ExtractAllBlueprintCreatedComponents( SourceMetahumanBlueprint.LoadSynchronous() );

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


				UBlueprintDataExtractionFL::CopyPropertiesFromOneObjectToAnother( SkeletalMeshComponent,
				                                                                  SkeletalMeshComponents[ SkeletalMeshComponentPropertyName ],
				                                                                  TArray<FName>{} );

			}
		}

		for ( auto GroomComponentPropertyName : GroomComponentPropertyNames )
		{
			if ( ComponentNameString.StartsWith( GroomComponentPropertyName.ToString() ) )
			{
				UBlueprintDataExtractionFL::CopyPropertiesFromOneObjectToAnother( GroomComponent,
				                                                                  GroomComponents[ GroomComponentPropertyName ],
				                                                                  TArray<FName>{} );
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

USceneComponent* UMetahumanComponentsDataAsset::GetSceneComponentByName(FName ComponentName) const
{
	if ( SkeletalMeshComponents.Contains( ComponentName ) )
	{
		return SkeletalMeshComponents[ ComponentName ];
	}
	else if ( GroomComponents.Contains( ComponentName ) )
	{
		return GroomComponents[ ComponentName ];
	}

	return nullptr;
}

bool UMetahumanComponentsDataAsset::HasSourceMetahumanBlueprint() const
{
	return IsValid( SourceMetahumanBlueprint.LoadSynchronous() );
}

TSoftObjectPtr<UBlueprint> UMetahumanComponentsDataAsset::GetSourceMetahumanBlueprint() const
{
	return SourceMetahumanBlueprint.LoadSynchronous();
}
