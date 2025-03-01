#pragma once

#include "Modules/ModuleManager.h"

class FMetahumanComponentDataExtractorEditorModule : public IModuleInterface
{
public:
	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	void OpenDataAssetWidget();

private:
	TSharedPtr<class FUICommandList> PluginCommandList;

private:
	void RegisterMenuButton();
};
