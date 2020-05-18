// Copyright (C) Gaslight Games Ltd, 2019-2020

// EOS Includes
#include "UEOSManager.h"
#include "Config/UEOSConfig.h"
#include "Authentication/Authentication.h"
#include "Metrics/Metrics.h"
#include "UserInfo/UserInfo.h"
#include "Friends/Friends.h"
#include "UEOSModule.h"

// EOS SDK Includes
#define ALLOW_RESERVED_PLATFORM_OPTIONS 0
#if ALLOW_RESERVED_PLATFORM_OPTIONS
#include "ReservedPlatformOptions.h"
#endif

// UE4 Includes
#include "Engine/Engine.h"
#include "Misc/Paths.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformFile.h"

// STD Includes
#include <string>

#define ALLOW_EOS_IN_PIE 0

// Static Initialization
UEOSManager* UEOSManager::EOSManager = nullptr;

UEOSManager::UEOSManager()
	: PlatformHandle( NULL )
	, bEOSInitialized( false )
	, bEOSShutdown( false )
	, Authentication( nullptr )
	, Metrics( nullptr )
	, Friends( nullptr )
	, UserInfo( nullptr )
{
	
}

bool UEOSManager::RequestEOSManager( UEOSManager*& ActiveEOSManager, UObject* WorldContextObject )
{
#if !ALLOW_EOS_IN_PIE
	UWorld* World = GEngine->GetWorldFromContextObjectChecked( WorldContextObject );
	if( World != nullptr )
	{
		if( World->WorldType == EWorldType::PIE )
		{
			FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] EOS Cannot be run whilst in PIE. Use Standalone or Debug from Visual Studio." ) );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

			return false;
		}
	}
#endif

	ActiveEOSManager = UEOSManager::GetEOSManager();
	return true;
}

UEOSManager* UEOSManager::GetEOSManager()
{
	if( UEOSManager::EOSManager == nullptr )
	{
		UEOSManager::EOSManager = NewObject<UEOSManager>();
		UEOSManager::EOSManager->AddToRoot();	// Prevent the GC from cleaning up our Instance
	}

	return UEOSManager::EOSManager;
}

void UEOSManager::Cleanup()
{
	if( UEOSManager::EOSManager != nullptr )
	{
		UEOSManager::EOSManager->ConditionalBeginDestroy();
	}
}

bool UEOSManager::IsEOSInitialized()
{
	if( UEOSManager::EOSManager != nullptr )
	{
		return UEOSManager::EOSManager->bEOSInitialized;
	}

	return false;
}

bool UEOSManager::HasEOSBeenShutdown()
{
	if( UEOSManager::EOSManager != nullptr )
	{
		return UEOSManager::EOSManager->bEOSShutdown;
	}

	return false;
}

EOS_HPlatform UEOSManager::GetPlatformHandle()
{
	return UEOSManager::EOSManager->PlatformHandle;
}

EEOSResults UEOSManager::InitEOS()
{
	if( IsEOSInitialized() )
	{
		return EEOSResults::ER_AlreadyInitialized;
	}

	if( bEOSShutdown == true )
	{
		return EEOSResults::ER_AlreadyShutdown;
	}

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	// Defaults - if none are set in the Config.
	std::string ProductName = "UEOS Plugin";
	std::string ProductVersion = "0.3";

	if( EOSConfig != nullptr )
	{
		ProductName = TCHAR_TO_UTF8( *EOSConfig->ProductName );
		ProductVersion = TCHAR_TO_UTF8( *EOSConfig->ProductVersion );
	}

	// Init EOS SDK
	EOS_InitializeOptions SDKOptions;
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = nullptr;
	SDKOptions.ReallocateMemoryFunction = nullptr;
	SDKOptions.ReleaseMemoryFunction = nullptr;
	SDKOptions.ProductName = ProductName.c_str();
	SDKOptions.ProductVersion = ProductVersion.c_str();
	SDKOptions.Reserved = nullptr;
	SDKOptions.SystemInitializeOptions = nullptr;

	EOS_EResult InitResult = EOS_Initialize( &SDKOptions );
	FString MessageText;

	if( InitResult != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Init Failed." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return UEOSCommon::GetResultsValue( InitResult );
	}

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Init Success!" ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	// Create platform instance
	EOS_Platform_Options PlatformOptions;
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.bIsServer = EOS_FALSE;
	PlatformOptions.EncryptionKey = nullptr;
	PlatformOptions.OverrideCountryCode = nullptr;
	PlatformOptions.OverrideLocaleCode = nullptr;
	static std::string EncryptionKey( 64, '1' );
	PlatformOptions.EncryptionKey = EncryptionKey.c_str();
	PlatformOptions.Flags = 0;

	FString TempPath = FPaths::ConvertRelativePathToFull( FPaths::ProjectSavedDir() + "/Temp/" );
	if( FPlatformFileManager::Get().GetPlatformFile().DirectoryExists( *TempPath ) == false )
	{
		if( FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree( *TempPath ) == false )
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Failed to create Cache Directory: %s." ), *TempPath );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

			return EEOSResults::ER_InvalidParameters;
		}
	}

	PlatformOptions.CacheDirectory = TCHAR_TO_UTF8( *TempPath );

	bool bHasInvalidParams = false;

	if( EOSConfig != nullptr )
	{
		PlatformOptions.bIsServer = ( EOSConfig->bIsServer ) ? EOS_TRUE : EOS_FALSE;

		std::string ProductId = TCHAR_TO_UTF8( *EOSConfig->ProductId );
		std::string SandboxId = TCHAR_TO_UTF8( *EOSConfig->SandboxId );
		std::string DeploymentId = TCHAR_TO_UTF8( *EOSConfig->DeploymentId );

		if( ProductId.empty() )
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] ProductId is invalid." ) );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

			bHasInvalidParams = true;
		}

		if( SandboxId.empty() )
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] SandboxId is invalid." ) );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

			bHasInvalidParams = true;
		}

		if( DeploymentId.empty() )
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] DeploymentId is invalid." ) );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

			bHasInvalidParams = true;
		}

		PlatformOptions.ProductId = ProductId.c_str();
		PlatformOptions.SandboxId = SandboxId.c_str();
		PlatformOptions.DeploymentId = DeploymentId.c_str();
		
		std::string ClientId = TCHAR_TO_UTF8( *EOSConfig->ClientId );
		std::string ClientSecret = TCHAR_TO_UTF8( *EOSConfig->ClientSecret );

		PlatformOptions.ClientCredentials.ClientId = ( EOSConfig->ClientId.IsEmpty() ) ? nullptr : ClientId.c_str();
		PlatformOptions.ClientCredentials.ClientSecret = ( EOSConfig->ClientSecret.IsEmpty() ) ? nullptr : ClientSecret.c_str();
		
#if ALLOW_RESERVED_PLATFORM_OPTIONS
		SetReservedPlatformOptions( PlatformOptions );
#else
		PlatformOptions.Reserved = NULL;
#endif // EOS_RESERVED_PLATFORM_OPTIONS

		if( bHasInvalidParams == true )
		{
			return EEOSResults::ER_MissingParameters;
		}

		PlatformHandle = EOS_Platform_Create( &PlatformOptions );
	}

	if( PlatformHandle == nullptr )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Platform Create - Failed!" ) );
		UE_LOG( UEOSLog, Warning, TEXT("%s"), *MessageText );

		return EEOSResults::ER_PlatformFailed;
	}
	

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Platform Create - Success!" ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Setting Logging Callback..." ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	EOS_EResult SetLogCallbackResult = EOS_Logging_SetCallback( &EOSSDKLoggingCallback );
	if( SetLogCallbackResult != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Set Logging Callback Failed!" ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return UEOSCommon::GetResultsValue( SetLogCallbackResult );
	}

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logging Callback Set." ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	EOS_ELogLevel EOSLogLevel = EOS_ELogLevel::EOS_LOG_VeryVerbose;
	if( EOSConfig != nullptr )
	{
		switch( EOSConfig->LogLevel )
		{
			case ELogLevel::LL_Off:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Off;
				break;
			case ELogLevel::LL_Fatal:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Fatal;
				break;
			case ELogLevel::LL_Error:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Error;
				break;
			case ELogLevel::LL_Warning:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Warning;
				break;
			case ELogLevel::LL_Info:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Info;
				break;
			case ELogLevel::LL_Verbose:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Verbose;
				break;
			case ELogLevel::LL_VeryVerbose:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_VeryVerbose;
				break;
			default:
				EOSLogLevel = EOS_ELogLevel::EOS_LOG_Off;
				break;
		}
	}

	EOS_Logging_SetLogLevel( EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOSLogLevel );

	bEOSInitialized = true;
	return EEOSResults::ER_Success;
}

EEOSResults UEOSManager::ShutdownEOS()
{
	if( !IsEOSInitialized() )
	{
		return EEOSResults::ER_NotInitialized;
	}

	if( bEOSShutdown == true )
	{
		return EEOSResults::ER_AlreadyShutdown;
	}

	if( PlatformHandle != nullptr )
	{
		EOS_Platform_Release( PlatformHandle );
	}

	EOS_EResult ShutdownResult = EOS_Shutdown();

	FString MessageText;
	EEOSResults eShutdownSuccess = UEOSCommon::GetResultsValue( ShutdownResult );

	if( ShutdownResult != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Shutdown Failed." ) );
	}
	else
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Shutdown Complete." ) );
		bEOSInitialized = false;
		bEOSShutdown = true;
	}

	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	return eShutdownSuccess;
}

bool UEOSManager::UpdateEOS()
{
	if( bEOSInitialized == true )
	{
		if( PlatformHandle != nullptr )
		{
			EOS_Platform_Tick( PlatformHandle );

			return true;
		}

		FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Unable to UpdateEOS - Platform Handle is NULL." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	}

	// Not logging if EOS hasn't been initialized - to avoid spamming the Log.

	return false;
}

UEOSAuthentication* UEOSManager::GetAuthentication()
{
	if( UEOSManager::EOSManager->Authentication == nullptr )
	{
		UEOSManager::EOSManager->Authentication = NewObject<UEOSAuthentication>( UEOSManager::EOSManager );
	}

	if( UEOSManager::EOSManager->Authentication == nullptr )
	{
		// Failed to instantiate the Authentication object.
		FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Failed to create Authentication Object." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	}

	return UEOSManager::EOSManager->Authentication;
}

UEOSMetrics* UEOSManager::GetMetrics()
{
	if( UEOSManager::EOSManager->Metrics == nullptr )
	{
		UEOSManager::EOSManager->Metrics = NewObject<UEOSMetrics>( UEOSManager::EOSManager );
	}

	if( UEOSManager::EOSManager->Metrics == nullptr )
	{
		// Failed to instantiate the Metrics object.
		FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Failed to create Metrics Object." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	}

	return UEOSManager::EOSManager->Metrics;
}

UEOSFriends* UEOSManager::GetFriends()
{
	if( UEOSManager::EOSManager->Friends == nullptr )
	{
		UEOSManager::EOSManager->Friends = NewObject<UEOSFriends>( UEOSManager::EOSManager );
	}

	if( UEOSManager::EOSManager->Friends == nullptr )
	{
		// Failed to instantiate the Friends object.
		FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Failed to create Friends Object." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	}

	return UEOSManager::EOSManager->Friends;
}

UEOSUserInfo* UEOSManager::GetUserInfo()
{
	if( UEOSManager::EOSManager->UserInfo == nullptr )
	{
		UEOSManager::EOSManager->UserInfo = NewObject<UEOSUserInfo>( UEOSManager::EOSManager );
	}

	if( UEOSManager::EOSManager->UserInfo == nullptr )
	{
		// Failed to instantiate the UserInfo object.
		FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Failed to create UserInfo Object." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	}

	return UEOSManager::EOSManager->UserInfo;
}

UEOSPresence* UEOSManager::GetPresence()
{
	if (UEOSManager::EOSManager->Presence == nullptr)
	{
		UEOSManager::EOSManager->Presence = NewObject<UEOSPresence>(UEOSManager::EOSManager);
	}

	if (UEOSManager::EOSManager->Presence == nullptr)
	{
		// Failed to instantiate the UserInfo object.
		FString MessageText = FString::Printf(TEXT("[EOS SDK | Plugin] Failed to create Presence Object."));
		UE_LOG(UEOSLog, Warning, TEXT("%s"), *MessageText);
	}

	return UEOSManager::EOSManager->Presence;
}

void UEOSManager::EOSSDKLoggingCallback( const EOS_LogMessage* InMsg )
{
	FString Message( InMsg->Message );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *Message );
}

FString UEOSManager::GetProductId()
{
	FString retVal = "";

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	if( EOSConfig != nullptr )
	{
		retVal = EOSConfig->ProductId;
	}

	return retVal;
}

FString UEOSManager::GetSandboxId()
{
	FString retVal = "";

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	if( EOSConfig != nullptr )
	{
		retVal = EOSConfig->SandboxId;
	}

	return retVal;
}

FString UEOSManager::GetSupportTicketingKey()
{
	FString retVal = "";

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	if( EOSConfig != nullptr )
	{
		retVal = EOSConfig->SupportTicketingKey;
	}

	return retVal;
}

FString UEOSManager::GetSupportTicketingURL()
{
	FString retVal = "";

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	if( EOSConfig != nullptr )
	{
		retVal = EOSConfig->SupportTicketingURL;
	}

	return retVal;
}

FString UEOSManager::GetClientId()
{
	FString retVal = "";

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	if( EOSConfig != nullptr )
	{
		retVal = EOSConfig->ClientId;
	}

	return retVal;
}

FString UEOSManager::GetClientSecret()
{
	FString retVal = "";

	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	if( EOSConfig != nullptr )
	{
		retVal = EOSConfig->ClientSecret;
	}

	return retVal;
}
