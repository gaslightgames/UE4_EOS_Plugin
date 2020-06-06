// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "OnlineSubsystemEOS.h"

// UE4 Includes
#include "Core.h"
#include "UObject/ObjectMacros.h"

// OSS EOS Includes
#include "OnlineIdentityInterfaceEOS.h"


IOnlineSessionPtr FOnlineSubsystemEOS::GetSessionInterface() const
{
	return nullptr;
}

IOnlineFriendsPtr FOnlineSubsystemEOS::GetFriendsInterface() const
{
	return nullptr;
}

IOnlinePartyPtr FOnlineSubsystemEOS::GetPartyInterface() const
{
	return nullptr;
}

IOnlineGroupsPtr FOnlineSubsystemEOS::GetGroupsInterface() const
{
	return nullptr;
}

IOnlineSharedCloudPtr FOnlineSubsystemEOS::GetSharedCloudInterface() const
{
	return nullptr;
}

IOnlineUserCloudPtr FOnlineSubsystemEOS::GetUserCloudInterface() const
{
	return nullptr;
}

IOnlineLeaderboardsPtr FOnlineSubsystemEOS::GetLeaderboardsInterface() const
{
	return nullptr;
}

IOnlineVoicePtr FOnlineSubsystemEOS::GetVoiceInterface() const
{
	return nullptr;
}

IOnlineExternalUIPtr FOnlineSubsystemEOS::GetExternalUIInterface() const
{
	return nullptr;
}

IOnlineTimePtr FOnlineSubsystemEOS::GetTimeInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemEOS::GetIdentityInterface() const
{
	return IdentityInterface;
}

IOnlineTitleFilePtr FOnlineSubsystemEOS::GetTitleFileInterface() const
{
	return nullptr;
}

IOnlineEntitlementsPtr FOnlineSubsystemEOS::GetEntitlementsInterface() const
{
	return nullptr;
}

IOnlineStorePtr FOnlineSubsystemEOS::GetStoreInterface() const
{
	return nullptr;
}

IOnlineStoreV2Ptr FOnlineSubsystemEOS::GetStoreV2Interface() const
{
	return nullptr; 
}

IOnlinePurchasePtr FOnlineSubsystemEOS::GetPurchaseInterface() const
{
	return nullptr;
}

IOnlineEventsPtr FOnlineSubsystemEOS::GetEventsInterface() const
{
	return nullptr;
}

IOnlineAchievementsPtr FOnlineSubsystemEOS::GetAchievementsInterface() const
{
	return nullptr;
}

IOnlineSharingPtr FOnlineSubsystemEOS::GetSharingInterface() const
{
	return nullptr;
}

IOnlineUserPtr FOnlineSubsystemEOS::GetUserInterface() const
{
	return nullptr;
}

IOnlineMessagePtr FOnlineSubsystemEOS::GetMessageInterface() const
{
	return nullptr;
}

IOnlinePresencePtr FOnlineSubsystemEOS::GetPresenceInterface() const
{
	return nullptr;
}

IOnlineChatPtr FOnlineSubsystemEOS::GetChatInterface() const
{
	return nullptr;
}

IOnlineStatsPtr FOnlineSubsystemEOS::GetStatsInterface() const
{
	return nullptr;
}

IOnlineTurnBasedPtr FOnlineSubsystemEOS::GetTurnBasedInterface() const
{
	return nullptr;
}

IOnlineTournamentPtr FOnlineSubsystemEOS::GetTournamentInterface() const
{
	return nullptr;
}

bool FOnlineSubsystemEOS::IsLocalPlayer( const FUniqueNetId& UniqueId ) const
{
	return false;
}

bool FOnlineSubsystemEOS::Init()
{
	if( IsEOSInitialized() == false )
	{
		// Attempt to initialize the SDK!
		if( GetEOSConfigOptions() == false )
		{
			UE_LOG_ONLINE( Warning, TEXT( "Could not gather EOS Config Options! Falling back to another OSS." ) );
			return false;
		}

		// Have config values, attempt to initialize the EOS SDK
		if( InitializeSDK() == false )
		{
			UE_LOG_ONLINE( Warning, TEXT( "Could not initialize EOS SDK! Falling back to another OSS." ) );
			return false;
		}
	}

	// Initialized the SDK, attempt to get a Platform Handle
	if( CreatePlatformHandle() == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Could not create Platform Handle from EOS SDK! Falling back to another OSS." ) );
		return false;
	}

	// Instantiate Online Subsystem interfaces
	IdentityInterface = MakeShareable( new FOnlineIdentityEOS( this ) );

	return true;
}

bool FOnlineSubsystemEOS::Shutdown()
{
	FOnlineSubsystemImpl::Shutdown();

	// Attempt to end any Async Processes.

#define DESTRUCT_INTERFACE(Interface) \
	if( Interface.IsValid() ) \
	{ \
		ensure( Interface.IsUnique() ); \
		Interface = nullptr; \
	}

	// Destroy Online Subsystem interfaces
	DESTRUCT_INTERFACE( IdentityInterface );

	if( IsEOSInitialized() == true )
	{
		// Attempt to Shutdown the SDK.
		EOS_EResult ShutdownResult = EOS_Shutdown();

		if( ShutdownResult != EOS_EResult::EOS_Success )
		{
			UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Shutdown failed!" ) );
			return false;
		}

		bEOSInitialized = false;
	}

	return true;
}

bool FOnlineSubsystemEOS::Exec( class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar )
{
	if( FOnlineSubsystemImpl::Exec( InWorld, Cmd, Ar ) )
	{
		return true;
	}

	return false;
}

bool FOnlineSubsystemEOS::IsEnabled() const
{
	return FOnlineSubsystemImpl::IsEnabled();
}

FString FOnlineSubsystemEOS::GetAppId() const
{
	return "";
}

FText FOnlineSubsystemEOS::GetOnlineServiceName() const
{
	return NSLOCTEXT( "OnlineSubsystemEOS", "OnlineServiceName", "EOS" );
}

bool FOnlineSubsystemEOS::Tick( float DeltaTime )
{
	if( IsEOSInitialized() == true )
	{
		if( PlatformHandle == nullptr )
		{
			UE_LOG_ONLINE( Warning, TEXT( "Can Tick EOS, PlatformHandle is invalid." ) );
			bEOSInitialized = false;
			return false;
		}

		EOS_Platform_Tick( PlatformHandle );
	}

	return true;
}

bool FOnlineSubsystemEOS::GetEOSConfigOptions()
{
	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "ProductName" ), ProductName, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing ProductName key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "ProductVersion" ), ProductVersion, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing ProductVersion key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "ProductId" ), ProductId, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing ProductId key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "SandboxId" ), SandboxId, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing SandboxId key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "DeploymentId" ), DeploymentId, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing DeploymentId key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "ClientId" ), ClientId, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing ClientId key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	if( GConfig->GetString( TEXT( "OnlineSubsystemEOS" ), TEXT( "ClientSecret" ), ClientSecret, GEngineIni ) == false )
	{
		UE_LOG_ONLINE( Warning, TEXT( "Missing ClientSecret key in OnlineSubsystemEOS of DefaultEngine.ini" ) );
		return false;
	}

	UE_LOG_ONLINE( Warning, TEXT( "EOS Config: Success." ) );

	return true;
}

bool FOnlineSubsystemEOS::InitializeSDK()
{
	FTCHARToUTF8 ProductNameStr( *ProductName );
	FTCHARToUTF8 ProductVersionStr( *ProductVersion );

	// Init EOS SDK
	EOS_InitializeOptions SDKOptions;
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = nullptr;
	SDKOptions.ReallocateMemoryFunction = nullptr;
	SDKOptions.ReleaseMemoryFunction = nullptr;
	SDKOptions.ProductName = ProductNameStr.Get();
	SDKOptions.ProductVersion = ProductVersionStr.Get();
	SDKOptions.Reserved = nullptr;
	SDKOptions.SystemInitializeOptions = nullptr;

	EOS_EResult InitResult = EOS_Initialize( &SDKOptions );

	if( InitResult != EOS_EResult::EOS_Success )
	{
		UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Initialization: FAILED!" ) );
		return false;
	}

	bEOSInitialized = true;
	UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Initialization: Success!" ) );
	return true;
}

bool FOnlineSubsystemEOS::CreatePlatformHandle()
{
	// Create platform instance
	EOS_Platform_Options PlatformOptions;
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.bIsServer = EOS_FALSE;
	PlatformOptions.EncryptionKey = nullptr;
	PlatformOptions.OverrideCountryCode = nullptr;
	PlatformOptions.OverrideLocaleCode = nullptr;
	// The EncryptionKey is a 64 ones
	static const char EncryptionKey[65] = "1111111111111111111111111111111111111111111111111111111111111111";
	PlatformOptions.EncryptionKey = EncryptionKey;
	PlatformOptions.Flags = 0;

	FString TempPath = FPaths::ConvertRelativePathToFull( FPaths::ProjectSavedDir() + "/Temp/" );
	if( FPlatformFileManager::Get().GetPlatformFile().DirectoryExists( *TempPath ) == false )
	{
		if( FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree( *TempPath ) == false )
		{
			UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Failed to create Cache Directory: %s." ), *TempPath );
			return false;
		}
	}

	PlatformOptions.CacheDirectory = TCHAR_TO_UTF8( *TempPath );

	FTCHARToUTF8 ProductIdStr( *ProductId );
	FTCHARToUTF8 SandboxIdStr( *SandboxId );
	FTCHARToUTF8 DeploymentIdStr( *DeploymentId );

	if( ProductIdStr.Length() == 0 )
	{
		UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Product Id is invalid." ) );
		return false;
	}

	if( SandboxIdStr.Length() == 0)
	{
		UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Sandbox Id is invalid." ) );
		return false;
	}

	if( DeploymentIdStr.Length() == 0 )
	{
		UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Deployment Id is invalid." ) );
		return false;
	}

	PlatformOptions.ProductId = ProductIdStr.Get();
	PlatformOptions.SandboxId = SandboxIdStr.Get();
	PlatformOptions.DeploymentId = DeploymentIdStr.Get();

	FTCHARToUTF8 ClientIdStr( *ClientId );
	FTCHARToUTF8 ClientSecretStr( *ClientSecret );

	PlatformOptions.ClientCredentials.ClientId = ClientIdStr.Get();
	PlatformOptions.ClientCredentials.ClientSecret = ClientSecretStr.Get();

	PlatformOptions.Reserved = NULL;

	PlatformHandle = EOS_Platform_Create( &PlatformOptions );

	if( PlatformHandle == nullptr )
	{
		UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Failed to create platform." ) );
		return false;
	}

	UE_LOG_ONLINE( Warning, TEXT( "EOS SDK Platform: Success." ) );

	return true;
}
