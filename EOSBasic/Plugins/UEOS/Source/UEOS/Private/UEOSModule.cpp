// Copyright (C) Gaslight Games Ltd, 2019

#include "UEOSModule.h"

// Settings
#include "Public/Config/UEOSConfig.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#endif

#define LOCTEXT_NAMESPACE "FUEOSModule"

void FUEOSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();
}

void FUEOSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if( UObjectInitialized() )
	{
		UnregisterSettings();
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE( FUEOSModule, UEOS )

DEFINE_LOG_CATEGORY( UEOSLog );
