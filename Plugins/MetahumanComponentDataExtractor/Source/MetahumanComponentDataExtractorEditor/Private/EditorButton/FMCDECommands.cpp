#include "MetahumanComponentDataExtractorEditor/Public/EditorButton/FMCDECommands.h"

#define LOCTEXT_NAMESPACE "FMetahumanComponentDataExtractorEditorModule"

void FMCDECommands::RegisterCommands()
{
	UI_COMMAND( PluginAction, "MetahumanComponentDataExtractor", "Execute MetahumanComponentDataExtractor action", EUserInterfaceActionType::Button, FInputChord() );
}

#undef LOCTEXT_NAMESPACE
