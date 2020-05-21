// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Tickable.h"

class FOnlineSubsystemEOSModule : public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void						StartupModule() override;
	virtual void						ShutdownModule() override;
	virtual bool						SupportsDynamicReloading() override;

	/**
	 * Are the EOS Dlls loaded
	 *
	 * @return if the EOS DLLs are currently loaded (if we are loading them dynamically, statically linked are always true)
	 */
	bool								AreEOSDllsLoaded() const;

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline class	FUEOSModule&	Get()
	{
		return FModuleManager::LoadModuleChecked<class FUEOSModule>( "OnlineSubsystemEOS" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool					IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "OnlineSubsystemEOS" );
	}

protected:

private:

	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryEOS*			EOSFactory;

	/** Handle to the DLL we will load */
	void*								EOSSDKHandle;

	/** StartupModule is covered with defines, these functions are the place to put breakpoints */
	static bool							LoadDependency( const FString& Dir, const FString& Name, void*& Handle );
	static void							FreeDependency( void*& Handle );
};

DECLARE_LOG_CATEGORY_EXTERN( OnlineSubsystemEOSLog, Log, All );
