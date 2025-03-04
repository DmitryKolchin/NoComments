// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Private/EditorFunctionLibrary.h"
#include "BlueprintDataExtractionEFL.generated.h"

/**
 * 
 */
UCLASS()
class METAHUMANCOMPONENTDATAEXTRACTOR_API UBlueprintDataExtractionEFL : public UEditorFunctionLibrary
{
	GENERATED_BODY()

public:
	static TArray<UActorComponent*> ExtractAllBlueprintCreatedComponents(UBlueprint* Blueprint);

	static void CopyPropertiesFromOneObjectToAnother(UObject* Source,
	                                                 UObject* Destination,
	                                                 const TArray<FName>& PropertiesToIgnore);
};
