// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NCCharacter_Base.generated.h"

class UNCMetaHumanComponent;
class ULODSyncComponent;
class UMetaHumanComponentUE;
class UMetahumanBuilderComponent;

DECLARE_LOG_CATEGORY_EXTERN( LogNCCharacter_Base, Log, All );

UCLASS()
class NOCOMMENTS_API ANCCharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	ANCCharacter_Base();

	virtual void PostActorCreated() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY( EditDefaultsOnly, Category="Components" )
	UMetahumanBuilderComponent* MetahumanBuilderComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	UNCMetaHumanComponent* MetaHumanComponent;

	UPROPERTY( EditDefaultsOnly, Category="Components" )
	ULODSyncComponent* LODSyncComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Eyelashes")
	FName EyelashesHiLODMaterialSlotName = TEXT("eyelashes_HiLOD_shader_shader");

	UPROPERTY(EditDefaultsOnly, Category = "Eyelashes")
	FName EyelashesMaterialSlotName = TEXT("eyelashes_shader_shader");

	UPROPERTY()
	bool bEnableMotionMatching = true;

	UPROPERTY(EditDefaultsOnly, Category="Walking")
	TSoftObjectPtr<UCurveFloat> StrafeSpeedMapCurve;

	UPROPERTY(EditDefaultsOnly, Category="Walking")
	FVector WalkSpeeds = FVector{200.f, 180.f, 150.f};


public:
	UMetahumanBuilderComponent* GetMetahumanBuilderComponent() const;

protected:
	void DisableMotionMatching();

	UCurveFloat* GetStrafeSpeedMapCurve() const;

	double CalculateMaxSpeedFromDirection(const FVector& Speeds) const;

private:
	void SetupHairLOD();

	void SetupLODSyncComponent();

	void SetupMovementComponent();

	void UpdateMovement_PreCMC();



	USkeletalMeshComponent* GetFaceSkeletalMeshComponent() const;
};
