#pragma once
#include "FMCDEStyle.h"

struct FMCDEStyle;

class FMCDECommands : public TCommands<FMCDECommands>
{
public:
	FMCDECommands() :
		TCommands<FMCDECommands>( TEXT( "MetahumanComponentDataExtractor" ),
		                          NSLOCTEXT( "Contexts", "MetahumanComponentDataExtractor", "Metahuman Component Data Extractor" ),
		                          NAME_None,
		                          FMCDEStyle::GetStyleSetName() )
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
