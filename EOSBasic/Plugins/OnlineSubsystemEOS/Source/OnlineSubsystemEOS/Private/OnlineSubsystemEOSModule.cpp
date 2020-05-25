// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "OnlineSubsystemEOSModule.h"

// UE4 Includes
#include "Core.h"
#include "Interfaces/IPluginManager.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"

// EOS Plugin Includes
#include "OnlineSubsystemEOS.h"


#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#endif

#define LOCTEXT_NAMESPACE "FUEOSModule"

#ifndef EOS_SDK_INSTALLED
#error Epic Online Services SDK not located! Expected to be found in <Your Project>/Plugins/UEOS/Source/ThirdParty
#endif // EOS_SDK_INSTALLED


/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryEOS : public IOnlineFactory
{

public:

	FOnlineFactoryEOS() {}
	virtual ~FOnlineFactoryEOS()
	{
		DestroySubsystem();
	}

	virtual IOnlineSubsystemPtr CreateSubsystem( FName InstanceName )
	{
		if( !EOSSingleton.IsValid() )
		{
			EOSSingleton = MakeShared<FOnlineSubsystemEOS, ESPMode::ThreadSafe>( InstanceName );
			if( EOSSingleton->IsEnabled() )
			{
				if( !EOSSingleton->Init() )
				{
					UE_LOG_ONLINE( Warning, TEXT( "EOS API failed to initialize!" ) );
					DestroySubsystem();
				}
			}
			else
			{
				UE_LOG_ONLINE( Warning, TEXT( "EOS API disabled!" ) );
				DestroySubsystem();
			}

			return EOSSingleton;
		}

		UE_LOG_ONLINE( Warning, TEXT( "Can't create more than one instance of EOS online subsystem!" ) );
		return nullptr;
	}

private:

	/** Single instantiation of the STEAM interface */
	static FOnlineSubsystemEOSPtr		EOSSingleton;

	virtual void						DestroySubsystem()
	{
		if( EOSSingleton.IsValid() )
		{
			EOSSingleton->Shutdown();
			EOSSingleton = nullptr;
		}
	}
};

FOnlineSubsystemEOSPtr FOnlineFactoryEOS::EOSSingleton = nullptr;


void FOnlineSubsystemEOSModule::StartupModule()
{
#if !PLATFORM_LINUX
#if defined( EOS_LIB )
	// Get the base directory of this plugin
	FString BaseDir = "";
	TSharedPtr<IPlugin> EOSOSS = IPluginManager::Get().FindPlugin( "OnlineSubsystemEOS" );
	
	if( EOSOSS != nullptr )
	{
		BaseDir = EOSOSS->GetBaseDir();
	}
	
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
	const FString LibName = TEXT( "libEOSSDK-Mac-Shipping" );
	const FString LibDir = FPaths::Combine( *SDKDir, TEXT( "Bin" ) );
#endif // WINDOWS/MAC

	if( LoadDependency( LibDir, LibName, EOSSDKHandle ) == false )
	{
		FMessageDialog::Open( EAppMsgType::Ok, LOCTEXT( LOCTEXT_NAMESPACE, "Failed to load EOS SDK. Plugin will not be functional." ) );
		FreeDependency( EOSSDKHandle );

		return;
	}

	// Create and register our singleton factory with the main online subsystem for easy access
	EOSFactory = new FOnlineFactoryEOS();

	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>( "OnlineSubsystem" );
	OSS.RegisterPlatformService( EOS_SUBSYSTEM, EOSFactory );

#endif // EOS_LIB
#endif // NOT LINUX
}

void FOnlineSubsystemEOSModule::ShutdownModule()
{
	// Free the dll handle
#if !PLATFORM_LINUX
#if defined( EOS_LIB )
	FreeDependency( EOSSDKHandle );
#endif
#endif
}

bool FOnlineSubsystemEOSModule::LoadDependency( const FString& Dir, const FString& Name, void*& Handle )
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

void FOnlineSubsystemEOSModule::FreeDependency( void*& Handle )
{
	if( Handle != nullptr )
	{
		FPlatformProcess::FreeDllHandle( Handle );
		Handle = nullptr;
	}
}

bool FOnlineSubsystemEOSModule::SupportsDynamicReloading()
{
	// Due to the loading of the DLLs and how the EOS API is initialized, we cannot support dynamic reloading.
	return false;
}

//void FUEOSModule::Tick( float DeltaTime )
//{
	//UEOSManager* EOSManager = UEOSManager::GetEOSManager();
	//
	//if( EOSManager != nullptr )
	//{
	//	EOSManager->UpdateEOS();
	//}
//}

bool FOnlineSubsystemEOSModule::AreEOSDllsLoaded() const
{
	return ( EOSSDKHandle != nullptr ) ? true : false;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FOnlineSubsystemEOSModule, OnlineSubsystemEOS )

DEFINE_LOG_CATEGORY( OnlineSubsystemEOSLog );
