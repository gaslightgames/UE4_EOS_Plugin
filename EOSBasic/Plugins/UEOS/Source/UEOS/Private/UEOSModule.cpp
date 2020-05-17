// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "UEOSModule.h"
#include "UEOSPrivatePCH.h"
#include "Interfaces/IPluginManager.h"

// EOS Plugin Includes
#include "Config/UEOSConfig.h"
#include "UEOSManager.h"


#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#endif

#define LOCTEXT_NAMESPACE "FUEOSModule"

void FUEOSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if !PLATFORM_LINUX
#if defined( EOS_LIB )
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin( "UEOS" )->GetBaseDir();
	const FString SDKDir = FPaths::Combine( *BaseDir, TEXT( "Source" ), TEXT( "ThirdParty" ), TEXT( "EOSSDK" ) );
	bool bLoaded = false;

#if PLATFORM_WINDOWS

#if PLATFORM_32BITS
	const FString LibName = TEXT( "EOSSDK-Win32-Shipping" );
#else
	const FString LibName = TEXT( "EOSSDK-Win64-Shipping" );
#endif
	const FString LibDir = FPaths::Combine( *SDKDir, TEXT( "Bin" ) );
#elif PLATFORM_MAC
	const FString LibName = TEXT( "EOSSDK-Mac-Shipping" );
	const FString LibDir = FPaths::Combine( *SDKDir, TEXT( "Bin" ) );
#endif // WINDOWS/MAC

	if( LoadDependency( LibDir, LibName, EOSSDKHandle ) == false )
	{
		FMessageDialog::Open( EAppMsgType::Ok, LOCTEXT( LOCTEXT_NAMESPACE, "Failed to load EOS SDK. Plug-in will not be functional." ) );
		FreeDependency( EOSSDKHandle );
	}
	else
	{
#if !WITH_EDITOR
		UEOSManager::GetEOSManager()->InitEOS();
#endif // WITH_EDITOR
	}

#endif // EOS_LIB
#endif // NOT LINUX

	RegisterSettings();
}

void FUEOSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
#if !PLATFORM_LINUX
#if defined( EOS_LIB )
	FreeDependency( EOSSDKHandle );
#endif
#endif
	if( UObjectInitialized() )
	{
		UnregisterSettings();
	}
}

bool FUEOSModule::LoadDependency( const FString& Dir, const FString& Name, void*& Handle )
{
	FString Lib = Name + TEXT( "." ) + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine( *Dir, *Lib );

	Handle = FPlatformProcess::GetDllHandle( *Path );

	if( Handle == nullptr )
	{
		return false;
	}

	return true;
}

void FUEOSModule::FreeDependency( void*& Handle )
{
	if( Handle != nullptr )
	{
		FPlatformProcess::FreeDllHandle( Handle );
		Handle = nullptr;
	}
}

bool FUEOSModule::SupportsDynamicReloading()
{
	return true;
}

bool FUEOSModule::HandleSettingsSaved()
{
	UEOSConfig* config = GetMutableDefault<UEOSConfig>();

	bool ResaveSettings = true;// false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if( ResaveSettings )
	{
		config->SaveConfig();
	}

	return true;
}

void FUEOSModule::RegisterSettings()
{

}

void FUEOSModule::UnregisterSettings()
{

}

void FUEOSModule::Tick( float DeltaTime )
{
	UEOSManager* EOSManager = UEOSManager::GetEOSManager();

	if( EOSManager != nullptr )
	{
		EOSManager->UpdateEOS();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FUEOSModule, UEOS )

DEFINE_LOG_CATEGORY( UEOSLog );
