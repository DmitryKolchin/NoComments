// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MetahumanBuilderComponentImportSettingsDataAsset.generated.h"

/**
 *  A wrapper struct for array of FNames to be put into a TMap
 */
USTRUCT()
struct FComponentDataImportSettings
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly )
	FName ComponentName = NAME_None;

	UPROPERTY( EditDefaultsOnly )
	TArray<FName> PropertiesToNotCopy = TArray<FName>{};

	static FComponentDataImportSettings EmptyComponentDataImportSettings;
};

/**
 *  Data Asset that holds the settings for Metahuman Builder Component importing components from metahumans
 *  Primarily created to hold the properties that are not supposed to be copied while importing components
 */
UCLASS()
class METAHUMANCOMPONENTDATAEXTRACTOR_API UMetahumanBuilderComponentImportSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY( EditDefaultsOnly, meta=(TitleProperty="ComponentName") )
	TArray<FComponentDataImportSettings> PropertiesToNotCopyForEachComponent;

public:
	FComponentDataImportSettings GetComponentDataImportSettingsForComponentName(const FName& ComponentName) const;
};
