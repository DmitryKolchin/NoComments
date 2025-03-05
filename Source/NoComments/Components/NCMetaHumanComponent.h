// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetaHumanComponentBase.h"

#include "NCMetaHumanComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOCOMMENTS_API UNCMetaHumanComponent : public UMetaHumanComponentBase
{
	GENERATED_BODY()

public:
	// UActorComponent interface
	virtual void BeginPlay() override;
	// End UActorComponent interface

private:
	void SetupCustomizableBodyPart(FMetaHumanCustomizableBodyPart& BodyPart);
	virtual void PostInitAnimBP(USkeletalMeshComponent* SkeletalMeshComponent, UAnimInstance* AnimInstance) const override final;

	/**
	 * The post-processing AnimBP to use for the body parts when either the physics asset or the control rig are set.
	 * Use the ABP_Clothing_PostProcess shipped along with MetaHumans. The MetaHuman component will control given variables
	 * to e.g. set the LOD thresholds.
	 */
	UPROPERTY( EditDefaultsOnly, Category = BodyParts )
	TSoftClassPtr<UAnimInstance> PostProcessAnimBP;
};
