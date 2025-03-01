// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetahumanBuilderComponent.generated.h"

class UGroomComponent;
class UMetahumanComponentsDataAsset;

UENUM( BlueprintType )
enum class EMetahumanComponentsCreationMode : uint8
{
	/* Spawns all new components */
	SpawnAllComponentsNew,
	/* Finds the first skeletal mesh component and uses it as body */
	UseSingleExistingSkeletalMeshAsBody,
};

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

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	UPROPERTY( EditInstanceOnly )
	TSoftObjectPtr<UMetahumanComponentsDataAsset> MetahumanComponentsDataAsset;

	UPROPERTY( EditDefaultsOnly )
	EMetahumanComponentsCreationMode MetahumanComponentsCreationMode = EMetahumanComponentsCreationMode::SpawnAllComponentsNew;

	UPROPERTY()
	TArray<UActorComponent*> SpawnedComponents;

	UPROPERTY()
	FName OwnerBodyComponentName = FName( "Body" );

public:
	UFUNCTION( BlueprintCallable )
	void AddComponentsToOwner(USkeletalMeshComponent* Body);

private:
	void AddNewSceneComponentToOwnerFromExistingSceneComponent(USceneComponent* ExistingComponent,
	                                                           USceneComponent* ParentComponent,
	                                                           FName ComponentPropertyName,
	                                                           const TSubclassOf<USceneComponent>& ComponentClass);

	void SetupOwnerBodySkeletalMeshComponent(USkeletalMeshComponent* Body);

	void AddSkeletalMeshComponentsToOwnerBody(USkeletalMeshComponent* Body);

	void AddGrooomComponentsToOwnerFace(USkeletalMeshComponent* Face);

	USkeletalMeshComponent* GetOwnerFaceSkeletalMeshComponent() const;

	void DestroySpawnedComponents();
};
