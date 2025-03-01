#include "MetahumanComponentDataExtractorEditor/Public/EditorButton/FMCDEStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMCDEStyle::StyleInstance = nullptr;

void FMCDEStyle::Initialize()
{
	if ( !StyleInstance.IsValid() )
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle( *StyleInstance );
	}
}

void FMCDEStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle( *StyleInstance );
	ensure( StyleInstance.IsUnique() );
	StyleInstance.Reset();
}

void FMCDEStyle::ReloadTextures()
{
	if ( FSlateApplication::IsInitialized() )
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMCDEStyle::Get()
{
	return *StyleInstance;
}

FName FMCDEStyle::GetStyleSetName()
{
	static FName StyleSetName( TEXT( "MCDEStyle" ) );
	return StyleSetName;
}

TSharedRef<class FSlateStyleSet> FMCDEStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable( new FSlateStyleSet( "MCDEStyle" ) );
	Style->SetContentRoot( IPluginManager::Get().FindPlugin( "MetahumanComponentDataExtractor" )->GetBaseDir() / TEXT( "Resources" ) );

	const FVector2D Icon20x20( 20.0f, 20.0f );
	Style->Set( "MetahumanComponentDataExtractor.PluginAction", new IMAGE_BRUSH_SVG( TEXT( "MetahumanIcon" ), Icon20x20 ) );

	return Style;
}
