// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetahumanBuilderComponent.generated.h"

class UMetahumanBuilderComponentImportSettingsDataAsset;
class UGroomComponent;
class UMetahumanComponentsDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METAHUMANCOMPONENTDATAEXTRACTOR_API UMetahumanBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMetahumanBuilderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if !UE_BUILD_SHIPPING
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY( EditAnywhere )
	TSoftObjectPtr<UMetahumanComponentsDataAsset> MetahumanComponentsDataAsset;

	UPROPERTY()
	TMap<FName, USceneComponent*> ManagedOwnerComponents;

	UPROPERTY(EditDefaultsOnly)
	FName BodySkeletalMeshOverrideName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMetahumanBuilderComponentImportSettingsDataAsset> MetahumanBuilderComponentImportSettingsDataAsset;

public:
	void InitializeManagedOwnerComponents();

	void SetMetahumanComponentsDataAsset(UMetahumanComponentsDataAsset* NewMetahumanComponentsDataAsset);

private:

	void InitializeOwnerBodyComponent();

	void InitializeManagedOwnerComponentsFromNamesAndClass(USceneComponent* ComponentToAttachTo, const TArray<FName>& ComponentNames, const TSubclassOf<USceneComponent>& ComponentClass);

	// Function that gets owner's body skeletal mesh component
	USkeletalMeshComponent* GetOrCreateOwnerBodyComponent();

	USceneComponent* AddSceneComponentToOwner(FName ComponentName, USceneComponent* ParentComponent, const TSubclassOf<USceneComponent>& ComponentClass);

	void DestroyManagedOwnerComponents();



};
