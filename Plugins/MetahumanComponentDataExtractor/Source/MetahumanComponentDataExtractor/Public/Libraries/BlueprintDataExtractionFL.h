// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataExtractionFL.generated.h"

/**
 * 
 */
UCLASS()
class METAHUMANCOMPONENTDATAEXTRACTOR_API UBlueprintDataExtractionFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TArray<UActorComponent*> ExtractAllBlueprintCreatedComponents(UBlueprint* Blueprint);

	static void CopyPropertiesFromOneObjectToAnother(UObject* Source,
	                                                 UObject* Destination,
	                                                 const TArray<FName>& PropertiesToIgnore);
};
