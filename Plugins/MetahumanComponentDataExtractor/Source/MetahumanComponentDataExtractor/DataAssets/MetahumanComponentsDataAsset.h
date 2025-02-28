// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MetahumanComponentsDataAsset.generated.h"

class UGroomComponent;

/**
 * 
 */
UCLASS( BlueprintType )
class METAHUMANCOMPONENTDATAEXTRACTOR_API UMetahumanComponentsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UMetahumanComponentsDataAsset();

private:
	UPROPERTY( EditDefaultsOnly, Instanced, Category="Skeletal Mesh Components" )
	TMap<FName, USkeletalMeshComponent*> SkeletalMeshComponents;

	UPROPERTY( EditDefaultsOnly, Instanced, Category="Groom Components" )
	TMap<FName, UGroomComponent*> GroomComponents;

	UPROPERTY()
	UBlueprint* SourceMetahumanBlueprint1 = nullptr;

public:
	UFUNCTION( BlueprintCallable, CallInEditor )
	void ExtractDataFromMetahumanBlueprint(UObject* Object);

	USkeletalMeshComponent* GetSkeletalMeshComponentByName(FName ComponentName) const;

	UGroomComponent* GetGroomComponentByName(FName ComponentName) const;

	UBlueprint* GetSourceMetahumanBlueprint() const;

private:
	void EmbedSourceMetahumanBlueprintThumbnail(UBlueprint* SourceMetahumanBlueprint);

};
