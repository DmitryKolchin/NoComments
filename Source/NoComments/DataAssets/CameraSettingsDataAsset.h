// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CameraSettingsDataAsset.generated.h"

USTRUCT( BlueprintType )
struct FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FFloatRange PitchRotationRange;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FFloatRange YawRotationRange;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	bool bIncreaseFOVWhenMoving = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIncreaseFOVWhenMoving") )
	float FOVIncreaseAmount = 10.0f;
};

/**
 * 
 */
UCLASS()
class NOCOMMENTS_API UCameraSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly )
	FCameraSettings DefaultCameraSettings;

	UPROPERTY( EditDefaultsOnly )
	FCameraSettings FightModeCameraSettings;


public:
	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Default Camera Settings" )FCameraSettings GetDefaultCameraSettings() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Fight Mode Camera Settings" )FCameraSettings GetFightModeCameraSettings() const;
};
