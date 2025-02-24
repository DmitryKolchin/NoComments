// Copyright Epic Games, Inc. All Rights Reserved.

#include "MetahumanComponentDataExtractor.h"

#include "MetahumanComponentDataExtractor/DataAssets/MetahumanComponentsDataAsset.h"
#include "MetahumanComponentDataExtractor/Editor/MCDEThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FMetahumanComponentDataExtractorModule"

void FMetahumanComponentDataExtractorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UThumbnailManager::Get().RegisterCustomRenderer(UMetahumanComponentsDataAsset::StaticClass(), UMCDEThumbnailRenderer::StaticClass());
}

void FMetahumanComponentDataExtractorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMetahumanComponentDataExtractorModule, MetahumanComponentDataExtractor)