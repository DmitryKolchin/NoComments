// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MetahumanComponentDataExtractorSettings.generated.h"

class UEditorUtilityWidgetBlueprint;
class UEditorUtilityWidget;
/**
 * 
 */
UCLASS( Config=Editor, defaultconfig )
class METAHUMANCOMPONENTDATAEXTRACTOR_API UMetahumanComponentDataExtractorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY( Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	TArray<FName> SkeletalMeshComponentPropertyNames;

	UPROPERTY( Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	TArray<FName> GroomComponentPropertyNames;

	UPROPERTY( Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	FName BodySkeletalMeshComponentPropertyName = "Body";

	UPROPERTY( Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	FName FaceSkeletalMeshComponentPropertyName = "Face";

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UEditorUtilityWidgetBlueprint> MetahumanComponentDataExtractorWidget;

public:
	const TArray<FName>& GetSkeletalMeshComponentPropertyNames() const;

	const TArray<FName>& GetGroomComponentPropertyNames() const;

	FName GetBodySkeletalMeshComponentPropertyName() const;

	FName GetFaceSkeletalMeshComponentPropertyName() const;

	TSoftObjectPtr<UEditorUtilityWidgetBlueprint> GetMetahumanComponentDataExtractorWidget() const;
};
