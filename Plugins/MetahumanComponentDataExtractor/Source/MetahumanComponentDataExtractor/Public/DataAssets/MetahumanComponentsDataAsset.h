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

	/** The source metahuman blueprint that this data asset is based on.
	 *  Please, change via the Editor Utility Widget.
	 */
	UPROPERTY(VisibleDefaultsOnly, Category="Source Metahuman Blueprint")
	TSoftObjectPtr<UBlueprint> SourceMetahumanBlueprint;

public:
	UFUNCTION( BlueprintCallable, CallInEditor )
	void ExtractDataFromMetahumanBlueprint(UObject* Object);

	USkeletalMeshComponent* GetSkeletalMeshComponentByName(FName ComponentName) const;

	UGroomComponent* GetGroomComponentByName(FName ComponentName) const;

	USceneComponent* GetSceneComponentByName(FName ComponentName) const;

	bool HasSourceMetahumanBlueprint() const;

	TSoftObjectPtr<UBlueprint> GetSourceMetahumanBlueprint() const;

};
