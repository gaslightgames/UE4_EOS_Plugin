// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class EOSSDK : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
    public EOSSDK(ReadOnlyTargetRules Target) : base(Target)
#else
    public EOSSDK(TargetInfo Target)
#endif
    {
        Type = ModuleType.External;

        string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "EOSSDK"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Include headers
            PublicIncludePaths.Add(Path.Combine(BaseDirectory, "Include"));

            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(BaseDirectory, "Lib"));
            PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Lib", "EOSSDK-Win64-Shipping.lib"));

            // Dlls
            RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Bin", "EOSSDK-Win64-Shipping.dll"));
            PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");
        }
        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            // Include headers
            PublicIncludePaths.Add(Path.Combine(BaseDirectory, "Include"));

            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(BaseDirectory, "Lib"));
            PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Lib", "EOSSDK-Win32-Shipping.lib"));

            // Dlls
            RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Bin", "EOSSDK-Win32-Shipping.dll"));
            PublicDelayLoadDLLs.Add("EOSSDK-Win32-Shipping.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            // Include headers
            PublicIncludePaths.Add(Path.Combine(BaseDirectory, "Include"));

            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(BaseDirectory, "Bin"));
            PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Bin", "libEOSSDK-Linux-Shipping.so"));
            RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Bin", "libEOSSDK-Linux-Shipping.so"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // Include headers
            PublicIncludePaths.Add(Path.Combine(BaseDirectory, "Include"));

            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(BaseDirectory, "Bin"));
            PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Bin", "EOSSDK-Mac-Shipping.dylib"));
            RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Bin", "EOSSDK-Mac-Shipping.dylib"));
        }
    }
}