using UnrealBuildTool;

public class MetahumanComponentDataExtractorEditor : ModuleRules
{
	public MetahumanComponentDataExtractorEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Blutility",
				"HairStrandsCore",
				"DeveloperSettings",
				"UnrealEd",
				"EditorFramework",
				"UMGEditor",
				"UMG",
				"ScriptableEditorWidgets"
				// ... add other public dependencies that you statically link with here ...
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"MetahumanComponentDataExtractor",
				// ... add private dependencies that you statically link with here ...
			}
		);
	}
}