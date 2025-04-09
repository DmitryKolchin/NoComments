// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MetahumanComponentDataExtractorEditorSettings.generated.h"

class UEditorUtilityWidgetBlueprint;
/**
 *
 */
UCLASS(defaultconfig, config = UMetahumanComponentDataExtractor, meta = (DisplayName = "Metahuman Component Data Extractor Editor Settings"))
class METAHUMANCOMPONENTDATAEXTRACTOREDITOR_API UMetahumanComponentDataExtractorEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UEditorUtilityWidgetBlueprint> MetahumanComponentDataExtractorWidget;

public:
	TSoftObjectPtr<UEditorUtilityWidgetBlueprint> GetMetahumanComponentDataExtractorWidget() const;
};
