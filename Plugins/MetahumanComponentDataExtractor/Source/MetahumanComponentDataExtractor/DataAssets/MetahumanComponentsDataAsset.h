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

public:
	UFUNCTION( BlueprintCallable, CallInEditor )
	void ExtractDataFromMetahumanBlueprint(UBlueprint* MetahumanBlueprint);

	USkeletalMeshComponent* GetSkeletalMeshComponentByName(FName ComponentName) const;

	UGroomComponent* GetGroomComponentByName(FName ComponentName) const;
};
