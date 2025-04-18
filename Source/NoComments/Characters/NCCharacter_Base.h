﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	void PostActorCreated() override;

protected:
	void BeginPlay() override;

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


public:
	UMetahumanBuilderComponent* GetMetahumanBuilderComponent() const;

private:
	void SetupHairLOD();

	USkeletalMeshComponent* GetFaceSkeletalMeshComponent() const;
};
