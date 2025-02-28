// Fill out your copyright notice in the Description page of Project Settings.

#include "MCDEThumbnailRenderer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "ObjectTools.h"
#include "MetahumanComponentDataExtractor/DataAssets/MetahumanComponentsDataAsset.h"

void UMCDEThumbnailRenderer::Draw(UObject* Object,
                                  int32 X,
                                  int32 Y,
                                  uint32 Width,
                                  uint32 Height,
                                  FRenderTarget* Viewport,
                                  FCanvas* Canvas,
                                  bool bAdditionalViewFamily)
{
	UMetahumanComponentsDataAsset* MetahumanComponentsDataAsset = Cast<UMetahumanComponentsDataAsset>( Object );

	if ( !IsValid( MetahumanComponentsDataAsset ) )
	{
		return;
	}

	UPackage* DataAssetPackage = MetahumanComponentsDataAsset->GetPackage();

	if ( !IsValid( DataAssetPackage ) )
	{
		return;
	}

	FString DataAssetPackageFullName = DataAssetPackage->GetPathName();
	FName DataAssetPackageFullNameAsFName = FName( *DataAssetPackageFullName );

	UBlueprint* SourceMetahumanBlueprint = MetahumanComponentsDataAsset->GetSourceMetahumanBlueprint();

	const FObjectThumbnail* ThumbnailCachedInDataAsset;

	if (DataAssetPackage->HasThumbnailMap())
	{
		ThumbnailCachedInDataAsset = DataAssetPackage->GetThumbnailMap().Find( DataAssetPackageFullNameAsFName );
	}
	else
	{
		ThumbnailCachedInDataAsset = IsValid( SourceMetahumanBlueprint )
		                             ? ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk( MetahumanComponentsDataAsset->GetSourceMetahumanBlueprint() )
		                             : nullptr;
	}

	if ( !ThumbnailCachedInDataAsset )
	{
		return;
	}

	int32 ThumbnailImageWidth = ThumbnailCachedInDataAsset->GetImageWidth();
	int32 ThumbnailImageHeight = ThumbnailCachedInDataAsset->GetImageHeight();

	if ( ThumbnailImageWidth <= 0 || ThumbnailImageHeight <= 0 )
	{
		return;
	}

	UTexture2D* ThumbnailTexture = UTexture2D::CreateTransient( ThumbnailImageWidth, ThumbnailImageHeight, PF_B8G8R8A8 );

	if ( !IsValid( ThumbnailTexture ) )
	{
		return;
	}

	ThumbnailTexture->MipGenSettings = TMGS_NoMipmaps;
	ThumbnailTexture->SRGB = true;
	ThumbnailTexture->AddToRoot();

	void* TextureData = ThumbnailTexture->GetPlatformData()->Mips[ 0 ].BulkData.Lock( LOCK_READ_WRITE );

	if ( !TextureData )
	{
		return;
	}

	FMemory::Memcpy( TextureData, ThumbnailCachedInDataAsset->AccessImageData().GetData(), ThumbnailCachedInDataAsset->AccessImageData().Num() );

	ThumbnailTexture->GetPlatformData()->Mips[ 0 ].BulkData.Unlock();
	ThumbnailTexture->UpdateResource();

	FCanvasTileItem TileItem( FVector2D( X, Y ), ThumbnailTexture->GetResource(), FVector2D( Width, Height ), FLinearColor::White );
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

bool UMCDEThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	return true;
}
