// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

#include "Modules/ModuleManager.h"
#include "Tickable.h"

class FUEOSModule : public IModuleInterface, public FTickableGameObject
{
public:

	/** IModuleInterface implementation */
	virtual void		StartupModule() override;
	virtual void		ShutdownModule() override;
	virtual bool		SupportsDynamicReloading() override;

	/** FTickableGameObject implementation */
	bool				IsTickable() const override { return true; }
	bool				IsTickableInEditor() const override { return true; }
	bool				IsTickableWhenPaused() const override { return true; }
	TStatId				GetStatId() const override
	{
		return TStatId();
	}

	void				Tick( float DeltaTime ) override;

protected:

	bool				HandleSettingsSaved();
	void				RegisterSettings();
	void				UnregisterSettings();

private:

	/** Handle to the test dll we will load */
	void*				EOSSDKHandle;

	/** StartupModule is covered with defines, these functions are the place to put breakpoints */
	static bool			LoadDependency( const FString& Dir, const FString& Name, void*& Handle );
	static void			FreeDependency( void*& Handle );
};

DECLARE_LOG_CATEGORY_EXTERN( UEOSLog, Log, All );
