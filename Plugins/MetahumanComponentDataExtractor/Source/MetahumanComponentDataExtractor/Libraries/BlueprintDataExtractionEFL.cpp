// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintDataExtractionEFL.h"

#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"

TArray<UActorComponent*> UBlueprintDataExtractionEFL::ExtractAllBlueprintCreatedComponents(UBlueprint* Blueprint)
{
	TArray<UActorComponent*> ExtractedComponents = TArray<UActorComponent*>{};

	if ( !IsValid( Blueprint ) )
	{
		FMessageDialog::Open( EAppMsgType::Ok, FText::FromString( "Provided Blueprint is invalid" ) );
		return ExtractedComponents;
	}

	USimpleConstructionScript* SimpleConstructionScript = Blueprint->SimpleConstructionScript;

	if ( !IsValid( SimpleConstructionScript ) )
	{
		FMessageDialog::Open( EAppMsgType::Ok, FText::FromString( "Provided Blueprint has no SimpleConstructionScript" ) );
		return ExtractedComponents;
	}

	for ( auto Node : SimpleConstructionScript->GetAllNodes() )
	{
		if ( !Node )
		{
			continue;
		}

		UActorComponent* ActorComponent = Cast<UActorComponent>( Node->ComponentTemplate );
		if ( IsValid( ActorComponent ) )
		{
			ExtractedComponents.Add( ActorComponent );
		}
	}

	return ExtractedComponents;
}

void UBlueprintDataExtractionEFL::CopyAllPropertiesFromOneObjectToAnother(UObject* Source, UObject* Destination)
{
	for ( TFieldIterator<FProperty> PropertyIterator( Source->GetClass() ); PropertyIterator; ++PropertyIterator )
	{
		FProperty* Property = *PropertyIterator;

		if ( !Property->HasAnyPropertyFlags( CPF_Edit | CPF_BlueprintVisible ) )
		{
			continue;
		}

		FString SerializedProperty;
		Property->ExportTextItem_Direct( SerializedProperty, Property->ContainerPtrToValuePtr<void>( Source ), nullptr, Source, PPF_None );

		Property->ImportText_Direct( *SerializedProperty, Property->ContainerPtrToValuePtr<void>( Destination ), Destination, PPF_None );
	}
}

