// Fill out your copyright notice in the Description page of Project Settings.

#include "MetahumanComponentsDataAsset.h"

#include "GroomComponent.h"
#include "MetahumanComponentDataExtractor/Libraries/BlueprintDataExtractionEFL.h"
#include "MetahumanComponentDataExtractor/Settings/MetahumanComponentDataExtractorSettings.h"

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

void UMetahumanComponentsDataAsset::ExtractDataFromMetahumanBlueprint(UBlueprint* MetahumanBlueprint)
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
	TArray<FName> GroomComponentPropertyNames = MetahumanComponentDataExtractorSettings->GetGroomComponentPropertyNames();

	TArray<UActorComponent*> ActorComponents = UBlueprintDataExtractionEFL::ExtractAllBlueprintCreatedComponents( MetahumanBlueprint );

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
