#include "MetahumanComponentDataExtractorEditor/Public/MetahumanComponentDataExtractorEditor.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "DataAssets/MetahumanComponentsDataAsset.h"
#include "EditorButton/FMCDECommands.h"
#include "MetahumanComponentDataExtractorEditor/Public/ThumbnailRenderer/MCDEThumbnailRenderer.h"
#include "Settings/MetahumanComponentDataExtractorEditorSettings.h"
#include "Settings/MetahumanComponentDataExtractorSettings.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FMetahumanComponentDataExtractorEditorModule"

void FMetahumanComponentDataExtractorEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UThumbnailManager::Get().RegisterCustomRenderer( UMetahumanComponentsDataAsset::StaticClass(), UMCDEThumbnailRenderer::StaticClass() );

	FMCDEStyle::Initialize();
	FMCDEStyle::ReloadTextures();

	FMCDECommands::Register();

	PluginCommandList = MakeShareable( new FUICommandList );

	PluginCommandList->MapAction(
		FMCDECommands::Get().PluginAction,
		FExecuteAction::CreateRaw( this, &FMetahumanComponentDataExtractorEditorModule::OpenDataAssetWidget ),
		FCanExecuteAction() );

	UToolMenus::RegisterStartupCallback( FSimpleMulticastDelegate::FDelegate::CreateRaw( this, &FMetahumanComponentDataExtractorEditorModule::RegisterMenuButton ) );
}

void FMetahumanComponentDataExtractorEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback( this );
	UToolMenus::UnregisterOwner( this );

	FMCDEStyle::Shutdown();

	FMCDECommands::Unregister();
}

void FMetahumanComponentDataExtractorEditorModule::OpenDataAssetWidget()
{
	const UMetahumanComponentDataExtractorEditorSettings* Settings = GetDefault<UMetahumanComponentDataExtractorEditorSettings>();
	{
		if ( !IsValid( Settings ) )
		{
			ensureAlwaysMsgf( false, TEXT( "FMetahumanComponentDataExtractorEditorModule::OpenDataAssetWidget: MetahumanComponentDataExtractorSettings is not valid." ) );
			return;
		}
	}

	UEditorUtilityWidgetBlueprint* EditorUtilityWidget = Settings->GetMetahumanComponentDataExtractorWidget().LoadSynchronous();

	if ( !EditorUtilityWidget )
	{
		ensureAlwaysMsgf( false, TEXT( "FMetahumanComponentDataExtractorEditorModule::OpenDataAssetWidget: EditorUtilityWidget is not valid." ) );
		return;
	}

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	{
		if ( !IsValid( EditorUtilitySubsystem ) )
		{
			ensureAlwaysMsgf( false, TEXT( "FMetahumanComponentDataExtractorEditorModule::OpenDataAssetWidget: EditorUtilitySubsystem is not valid." ) );
			return;
		}
	}

	EditorUtilitySubsystem->SpawnAndRegisterTab( EditorUtilityWidget );
}

void FMetahumanComponentDataExtractorEditorModule::RegisterMenuButton()
{
	FToolMenuOwnerScoped OwnerScoped( this );

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu( "LevelEditor.MainMenu.Window" );
		{
			FToolMenuSection& Section = Menu->FindOrAddSection( "MCDE_WindowLayout" );
			Section.AddMenuEntryWithCommandList( FMCDECommands::Get().PluginAction, PluginCommandList );
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu( "LevelEditor.LevelEditorToolBar.PlayToolBar" );
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection( "MCDE_PluginTools" );
			{
				FToolMenuEntry& Entry = Section.AddEntry( FToolMenuEntry::InitToolBarButton( FMCDECommands::Get().PluginAction ) );
				Entry.SetCommandList( PluginCommandList );
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMetahumanComponentDataExtractorEditorModule, MetahumanComponentDataExtractor)