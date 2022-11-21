using UnrealBuildTool;


public class Matchmaking : ModuleRules
{
	public Matchmaking(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new []{"Core","CoreUObject", "Engine", "PlayFabTutorial", "PlayFabCommon", "PlayFabCpp"});
	}
}