// (C) Gaslight Games Ltd, 2017-2019.  All rights reserved.

using UnrealBuildTool;
using System.IO;

public class UEOS : ModuleRules
{
    public UEOS( ReadOnlyTargetRules Target ) : base( Target )
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
               // "UEOS/Public",
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "UEOS/Private",
                "../ThirdParty/Include",
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        string BaseDirectory = Path.GetFullPath( Path.Combine( ModuleDirectory, "..", ".." ) );
        string LibPath = Path.Combine( BaseDirectory, "ThirdParty", "Lib" );

        // Add compile time libs
        PublicAdditionalLibraries.Add( Path.Combine( LibPath, "EOSSDK-Win64-Shipping.lib" ) );

        // Add runtime dependencies
        string DLLPath = Path.Combine( BaseDirectory, "ThirdParty", "Bin" );

        if( Target.Platform == UnrealTargetPlatform.Win64 )
        {
            RuntimeDependencies.Add( Path.Combine( DLLPath, "EOSSDK-Win64-Shipping.dll" ) );
        }
    }
}
