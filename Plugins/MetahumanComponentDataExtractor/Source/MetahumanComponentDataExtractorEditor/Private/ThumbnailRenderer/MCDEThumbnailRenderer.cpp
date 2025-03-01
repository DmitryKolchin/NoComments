// Fill out your copyright notice in the Description page of Project Settings.

#include "ThumbnailRenderer/MCDEThumbnailRenderer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "ObjectTools.h"
#include "DataAssets/MetahumanComponentsDataAsset.h"

class UMetahumanComponentsDataAsset;

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

	const FObjectThumbnail* DataAssetAssociatedThumbnail = GetThumbnailForDataAsset( MetahumanComponentsDataAsset );

	// If we have no thumbnail to draw - we don't draw anything
	if ( !DataAssetAssociatedThumbnail )
	{
		return;
	}

	// Now we can generate the thumbnail texture from the cached image data
	UTexture2D* ThumbnailTexture = GetThumbnailTextureFromObjectThumbnail( DataAssetAssociatedThumbnail );

	FCanvasTileItem TileItem( FVector2D( X, Y ), ThumbnailTexture->GetResource(), FVector2D( Width, Height ), FLinearColor::White );
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

bool UMCDEThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	return true;
}

const FObjectThumbnail* UMCDEThumbnailRenderer::GetThumbnailForDataAsset(UMetahumanComponentsDataAsset* MetahumanComponentsDataAsset) const
{
	{
		if ( !IsValid( MetahumanComponentsDataAsset ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMCDEThumbnailRenderer::GetThumbnailForDataAsset: MetahumanComponentsDataAsset is not valid." ) );
			return nullptr;
		}
	}

	UPackage* DataAssetPackage = MetahumanComponentsDataAsset->GetPackage();

	{
		if ( !IsValid( DataAssetPackage ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMCDEThumbnailRenderer::GetThumbnailForDataAsset: DataAssetPackage is not valid." ) );
			return nullptr;
		}
	}

	FString DataAssetPackageFullName = DataAssetPackage->GetPathName();
	FName DataAssetPackageFullNameAsFName = FName( *DataAssetPackageFullName );

	// Trying to get thumbnail from the data asset package
	// If we have already thumbnail cached - just use it
	if ( DataAssetPackage->HasThumbnailMap() )
	{
		return DataAssetPackage->GetThumbnailMap().Find( DataAssetPackageFullNameAsFName );
	}

	// If we don't have a cached thumbnail, but we have a source metahuman blueprint - we ask data asset to update the thumbnail
	if ( MetahumanComponentsDataAsset->HasSourceMetahumanBlueprint() )
	{
		MetahumanComponentsDataAsset->UpdateEmbeddedThumbnail();

		{
			//Assert for impossible situation
			if ( !DataAssetPackage->HasThumbnailMap() )
			{
				ensureAlwaysMsgf( false, TEXT( "UMCDEThumbnailRenderer::Draw: DataAssetPackage has no thumbnail map." ) );
				return nullptr;
			}
		}

		return DataAssetPackage->GetThumbnailMap().Find( DataAssetPackageFullNameAsFName );
	}
	// Otherwise we don't have a thumbnail to draw
	return nullptr;
}

UTexture2D* UMCDEThumbnailRenderer::GetThumbnailTextureFromObjectThumbnail(const FObjectThumbnail* ObjectThumbnail) const
{
	{
		if ( !ObjectThumbnail )
		{
			ensureAlwaysMsgf( false, TEXT( "UMCDEThumbnailRenderer::CreateThumbnailTextureFromObjectThumbnail: ObjectThumbnail is not valid." ) );
			return nullptr;
		}
	}

	int32 ThumbnailImageWidth = ObjectThumbnail->GetImageWidth();
	int32 ThumbnailImageHeight = ObjectThumbnail->GetImageHeight();

	{
		if ( ThumbnailImageWidth <= 0 || ThumbnailImageHeight <= 0 )
		{
			ensureAlwaysMsgf( false, TEXT( "UMCDEThumbnailRenderer::CreateThumbnailTextureFromObjectThumbnail: ThumbnailImageWidth or ThumbnailImageHeight is not valid." ) );
			return nullptr;
		}
	}

	// Now we can generate the thumbnail texture from the given thumbnail image data
	UTexture2D* ThumbnailTexture = UTexture2D::CreateTransient( ThumbnailImageWidth, ThumbnailImageHeight, PF_B8G8R8A8 );

	{
		if ( !IsValid( ThumbnailTexture ) )
		{
			ensureAlwaysMsgf( false, TEXT( "UMCDEThumbnailRenderer::CreateThumbnailTextureFromObjectThumbnail: ThumbnailTexture is not valid." ) );
			return nullptr;
		}
	}

	ThumbnailTexture->MipGenSettings = TMGS_NoMipmaps;
	ThumbnailTexture->SRGB = true;
	ThumbnailTexture->AddToRoot();

	void* TextureData = ThumbnailTexture->GetPlatformData()->Mips[ 0 ].BulkData.Lock( LOCK_READ_WRITE );

	if ( !TextureData )
	{
		return nullptr;
	}

	// Copying the thumbnail image data to the newly created texture
	FMemory::Memcpy( TextureData, ObjectThumbnail->AccessImageData().GetData(), ObjectThumbnail->AccessImageData().Num() );

	ThumbnailTexture->GetPlatformData()->Mips[ 0 ].BulkData.Unlock();
	ThumbnailTexture->UpdateResource();

	return ThumbnailTexture;
}
