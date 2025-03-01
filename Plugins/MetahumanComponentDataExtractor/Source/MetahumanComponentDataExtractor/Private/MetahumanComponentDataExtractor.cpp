// Copyright Epic Games, Inc. All Rights Reserved.

#include "MetahumanComponentDataExtractor.h"

#define LOCTEXT_NAMESPACE "FMetahumanComponentDataExtractorModule"

void FMetahumanComponentDataExtractorModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FMetahumanComponentDataExtractorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FMetahumanComponentDataExtractorModule, MetahumanComponentDataExtractor )
