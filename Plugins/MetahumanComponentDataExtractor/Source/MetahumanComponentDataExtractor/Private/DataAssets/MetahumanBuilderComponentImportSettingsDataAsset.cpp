// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/MetahumanBuilderComponentImportSettingsDataAsset.h"

FComponentDataImportSettings FComponentDataImportSettings::EmptyComponentDataImportSettings = FComponentDataImportSettings{};

FComponentDataImportSettings UMetahumanBuilderComponentImportSettingsDataAsset::GetComponentDataImportSettingsForComponentName(const FName& ComponentName) const
{
	for ( const FComponentDataImportSettings& ComponentDataImportSettings : PropertiesToNotCopyForEachComponent )
	{
		if ( ComponentDataImportSettings.ComponentName.IsEqual( ComponentName ) )
		{
			return ComponentDataImportSettings;
		}
	}

	return FComponentDataImportSettings::EmptyComponentDataImportSettings;
}
