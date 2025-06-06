﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "MCDEThumbnailRenderer.generated.h"

class UMetahumanComponentsDataAsset;
/**
 *
 */
UCLASS()
class METAHUMANCOMPONENTDATAEXTRACTOREDITOR_API UMCDEThumbnailRenderer : public UThumbnailRenderer
{
	GENERATED_BODY()

public:
	virtual void Draw(UObject* Object,
	                  int32 X,
	                  int32 Y,
	                  uint32 Width,
	                  uint32 Height,
	                  FRenderTarget* Viewport,
	                  FCanvas* Canvas,
	                  bool bAdditionalViewFamily) override;

	virtual bool CanVisualizeAsset(UObject* Object) override;

	UFUNCTION(BlueprintCallable)
	static void UpdateEmbeddedThumbnailFromSourceMetahumanBlueprint(UMetahumanComponentsDataAsset* MetahumanComponentsDataAsset);

private:
	const FObjectThumbnail* GetThumbnailForDataAsset(UMetahumanComponentsDataAsset* MetahumanComponentsDataAsset) const;

	UTexture2D* GetThumbnailTextureFromObjectThumbnail(const FObjectThumbnail* ObjectThumbnail) const;


};
