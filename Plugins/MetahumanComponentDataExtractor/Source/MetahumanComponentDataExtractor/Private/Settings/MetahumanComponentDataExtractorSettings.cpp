// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/MetahumanComponentDataExtractorSettings.h"

const TArray<FName>& UMetahumanComponentDataExtractorSettings::GetSkeletalMeshComponentPropertyNames() const
{
	return SkeletalMeshComponentPropertyNames;
}

const TArray<FName>& UMetahumanComponentDataExtractorSettings::GetGroomComponentPropertyNames() const
{
	return GroomComponentPropertyNames;
}

FName UMetahumanComponentDataExtractorSettings::GetBodySkeletalMeshComponentPropertyName() const
{
	return BodySkeletalMeshComponentPropertyName;
}

FName UMetahumanComponentDataExtractorSettings::GetFaceSkeletalMeshComponentPropertyName() const
{
	return FaceSkeletalMeshComponentPropertyName;
}
