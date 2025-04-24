// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraSettingsDataAsset.h"

FCameraSettings UCameraSettingsDataAsset::GetDefaultCameraSettings() const
{
	return DefaultCameraSettings;
}

FCameraSettings UCameraSettingsDataAsset::GetFightModeCameraSettings() const
{
	return FightModeCameraSettings;
}
