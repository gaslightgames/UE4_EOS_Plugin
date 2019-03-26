// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

#include "Engine.h"

class FUEOSModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;

protected:

	bool HandleSettingsSaved();
	void RegisterSettings();
	void UnregisterSettings();
};

DECLARE_LOG_CATEGORY_EXTERN( UEOSLog, Log, All );
