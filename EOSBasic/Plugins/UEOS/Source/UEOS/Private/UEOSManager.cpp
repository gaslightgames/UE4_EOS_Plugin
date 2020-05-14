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

FString UEOSManager::EOSResultToString( EOS_EResult Result )
{
	switch( Result )
	{

		case EOS_EResult::EOS_Success:
			return "EOS_Success";
		case EOS_EResult::EOS_NoConnection:
			return "EOS_NoConnection";
		case EOS_EResult::EOS_InvalidCredentials:
			return "EOS_InvalidCredentials";
		case EOS_EResult::EOS_InvalidUser:
			return "EOS_InvalidUser";
		case EOS_EResult::EOS_InvalidAuth:
			return "EOS_InvalidAuth";
		case EOS_EResult::EOS_AccessDenied:
			return "EOS_AccessDenied";
		case EOS_EResult::EOS_MissingPermissions:
			return "EOS_MissingPermissions";
		case EOS_EResult::EOS_Token_Not_Account:
			return "EOS_Token_Not_Account";
		case EOS_EResult::EOS_TooManyRequests:
			return "EOS_TooManyRequests";
		case EOS_EResult::EOS_AlreadyPending:
			return "EOS_AlreadyPending";
		case EOS_EResult::EOS_InvalidParameters:
			return "EOS_InvalidParameters";
		case EOS_EResult::EOS_InvalidRequest:
			return "EOS_InvalidRequest";
		case EOS_EResult::EOS_UnrecognizedResponse:
			return "EOS_UnrecognizedResponse";
		case EOS_EResult::EOS_IncompatibleVersion:
			return "EOS_IncompatibleVersion";
		case EOS_EResult::EOS_NotConfigured:
			return "EOS_NotConfigured";
		case EOS_EResult::EOS_AlreadyConfigured:
			return "EOS_AlreadyConfigured";
		case EOS_EResult::EOS_NotImplemented:
			return "EOS_NotImplemented";
		case EOS_EResult::EOS_Canceled:
			return "EOS_Canceled";
		case EOS_EResult::EOS_NotFound:
			return "EOS_NotFound";
		case EOS_EResult::EOS_OperationWillRetry:
			return "EOS_OperationWillRetry";
		case EOS_EResult::EOS_NoChange:
			return "EOS_NoChange";
		case EOS_EResult::EOS_VersionMismatch:
			return "EOS_VersionMismatch";
		case EOS_EResult::EOS_LimitExceeded:
			return "EOS_LimitExceeded";
		case EOS_EResult::EOS_Disabled:
			return "EOS_Disabled";
		case EOS_EResult::EOS_DuplicateNotAllowed:
			return "EOS_DuplicateNotAllowed";
		case EOS_EResult::EOS_MissingParameters_DEPRECATED:
			return "EOS_MissingParameters";
		case EOS_EResult::EOS_InvalidSandboxId:
			return "EOS_InvalidSandboxId";
		case EOS_EResult::EOS_TimedOut:
			return "EOS_TimedOut";
		case EOS_EResult::EOS_PartialResult:
			return "EOS_PartialResult";
		case EOS_EResult::EOS_Missing_Role:
			return "EOS_Missing_Role";
		case EOS_EResult::EOS_Missing_Feature:
			return "EOS_Missing_Feature";
		case EOS_EResult::EOS_Invalid_Sandbox:
			return "EOS_Invalid_Sandbox";
		case EOS_EResult::EOS_Invalid_Deployment:
			return "EOS_Invalid_Deployment";
		case EOS_EResult::EOS_Invalid_Product:
			return "EOS_Invalid_Product";
		case EOS_EResult::EOS_Invalid_ProductUserID:
			return "EOS_Invalid_ProductUserID";
		case EOS_EResult::EOS_ServiceFailure:
			return "EOS_ServiceFailure";
		case EOS_EResult::EOS_CacheDirectoryMissing:
			return "EOS_CacheDirectoryMissing";
		case EOS_EResult::EOS_CacheDirectoryInvalid:
			return "EOS_CacheDirectoryInvalid";
		case EOS_EResult::EOS_InvalidState:
			return "EOS_InvalidState";

		case EOS_EResult::EOS_Auth_AccountLocked:
			return "EOS_Auth_AccountLocked";
		case EOS_EResult::EOS_Auth_AccountLockedForUpdate:
			return "EOS_Auth_AccountLockedForUpdate";
		case EOS_EResult::EOS_Auth_InvalidRefreshToken:
			return "EOS_Auth_InvalidRefreshToken";
		case EOS_EResult::EOS_Auth_InvalidToken:
			return "EOS_Auth_InvalidToken";
		case EOS_EResult::EOS_Auth_AuthenticationFailure:
			return "EOS_Auth_AuthenticationFailure";
		case EOS_EResult::EOS_Auth_InvalidPlatformToken:
			return "EOS_Auth_InvalidPlatformToken";
		case EOS_EResult::EOS_Auth_WrongAccount:
			return "EOS_Auth_WrongAccount";
		case EOS_EResult::EOS_Auth_WrongClient:
			return "EOS_Auth_WrongClient";
		case EOS_EResult::EOS_Auth_FullAccountRequired:
			return "EOS_Auth_FullAccountRequired";
		case EOS_EResult::EOS_Auth_HeadlessAccountRequired:
			return "EOS_Auth_HeadlessAccountRequired";
		case EOS_EResult::EOS_Auth_PasswordResetRequired:
			return "EOS_Auth_PasswordResetRequired";
		case EOS_EResult::EOS_Auth_PasswordCannotBeReused:
			return "EOS_Auth_PasswordCannotBeReused";
		case EOS_EResult::EOS_Auth_Expired:
			return "EOS_Auth_Expired";
		case EOS_EResult::EOS_Auth_ScopeConsentRequired:
			return "EOS_Auth_ScopeConsentRequired";
		case EOS_EResult::EOS_Auth_ApplicationNotFound:
			return "EOS_Auth_ApplicationNotFound";
		case EOS_EResult::EOS_Auth_ScopeNotFound:
			return "EOS_Auth_ScopeNotFound";
		case EOS_EResult::EOS_Auth_AccountFeatureRestricted:
			return "EOS_Auth_AccountFeatureRestricted";
		case EOS_EResult::EOS_Auth_PersistentAuth_AccountNotActive:
			return "EOS_AccountNotActive";

		case EOS_EResult::EOS_Auth_PinGrantCode:
			return "EOS_Auth_PinGrantCode";
		case EOS_EResult::EOS_Auth_PinGrantExpired:
			return "EOS_Auth_PinGrantExpired";
		case EOS_EResult::EOS_Auth_PinGrantPending:
			return "EOS_Auth_PinGrantPending";

		case EOS_EResult::EOS_Auth_ExternalAuthNotLinked:
			return "EOS_Auth_ExternalAuthNotLinked";
		case EOS_EResult::EOS_Auth_ExternalAuthRevoked:
			return "EOS_Auth_ExternalAuthRevoked";
		case EOS_EResult::EOS_Auth_ExternalAuthInvalid:
			return "EOS_Auth_ExternalAuthInvalid";
		case EOS_EResult::EOS_Auth_ExternalAuthRestricted:
			return "EOS_Auth_ExternalAuthRestricted";
		case EOS_EResult::EOS_Auth_ExternalAuthCannotLogin:
			return "EOS_Auth_ExternalAuthCannotLogin";
		case EOS_EResult::EOS_Auth_ExternalAuthExpired:
			return "EOS_Auth_ExternalAuthExpired";
		case EOS_EResult::EOS_Auth_ExternalAuthIsLastLoginType:
			return "EOS_Auth_ExternalAuthIsLastLoginType";

		case EOS_EResult::EOS_Auth_ExchangeCodeNotFound:
			return "EOS_Auth_ExchangeCodeNotFound";
		case EOS_EResult::EOS_Auth_OriginatingExchangeCodeSessionExpired:
			return "EOS_Auth_OriginatingExchangeCodeSessionExpired";
		
		case EOS_EResult::EOS_Auth_MFARequired:
			return "EOS_Auth_MFARequired";

		case EOS_EResult::EOS_Auth_ParentalControls:
			return "EOS_Auth_ParentalControls";

		case EOS_EResult::EOS_Auth_NoRealId:
			return "EOS_Auth_NoRealId";

		case EOS_EResult::EOS_Friends_InviteAwaitingAcceptance:
			return "EOS_Friends_InviteAwaitingAcceptance";
		case EOS_EResult::EOS_Friends_NoInvitation:
			return "EOS_Friends_NoInvitation";
		case EOS_EResult::EOS_Friends_AlreadyFriends:
			return "EOS_Friends_AlreadyFriends";
		case EOS_EResult::EOS_Friends_NotFriends:
			return "EOS_Friends_NotFriends";
		case EOS_EResult::EOS_Friends_TargetUserTooManyInvites:
			return "EOS_Friends_TargetUserTooManyInvites";
		case EOS_EResult::EOS_Friends_LocalUserTooManyInvites:
			return "EOS_Friends_LocalUserTooManyInvites";
		case EOS_EResult::EOS_Friends_TargetUserFriendLimitExceeded:
			return "EOS_Friends_TargetUserFriendLimitExceeded";
		case EOS_EResult::EOS_Friends_LocalUserFriendLimitExceeded:
			return "EOS_Friends_LocalUserFriendLimitExceeded";

		case EOS_EResult::EOS_Presence_DataInvalid:
			return "EOS_Presence_DataInvalid";
		case EOS_EResult::EOS_Presence_DataLengthInvalid:
			return "EOS_Presence_DataLengthInvalid";
		case EOS_EResult::EOS_Presence_DataKeyInvalid:
			return "EOS_Presence_DataKeyInvalid";
		case EOS_EResult::EOS_Presence_DataKeyLengthInvalid:
			return "EOS_Presence_DataKeyLengthInvalid";
		case EOS_EResult::EOS_Presence_DataValueInvalid:
			return "EOS_Presence_DataValueInvalid";
		case EOS_EResult::EOS_Presence_DataValueLengthInvalid:
			return "EOS_Presence_DataValueLengthInvalid";
		case EOS_EResult::EOS_Presence_RichTextInvalid:
			return "EOS_Presence_RichTextInvalid";
		case EOS_EResult::EOS_Presence_RichTextLengthInvalid:
			return "EOS_Presence_RichTextLengthInvalid";
		case EOS_EResult::EOS_Presence_StatusInvalid:
			return "EOS_Presence_StatusInvalid";

		case EOS_EResult::EOS_Ecom_EntitlementStale:
			return "EOS_Ecom_EntitlementStale";
		case EOS_EResult::EOS_Ecom_CatalogOfferStale:
			return "EOS_Ecom_CatalogOfferStale";
		case EOS_EResult::EOS_Ecom_CatalogItemStale:
			return "EOS_Ecom_CatalogItemStale";
		case EOS_EResult::EOS_Ecom_CatalogOfferPriceInvalid:
			return "EOS_Ecom_CatalogOfferPriceInvalid";
		case EOS_EResult::EOS_Ecom_CheckoutLoadError:
			return "EOS_Ecom_CheckoutLoadError";

		case EOS_EResult::EOS_Sessions_SessionInProgress:
			return "EOS_Sessions_SessionInProgress";
		case EOS_EResult::EOS_Sessions_TooManyPlayers:
			return "EOS_Sessions_TooManyPlayers";
		case EOS_EResult::EOS_Sessions_NoPermission:
			return "EOS_Sessions_NoPermission";
		case EOS_EResult::EOS_Sessions_SessionAlreadyExists:
			return "EOS_Sessions_SessionAlreadyExists";
		case EOS_EResult::EOS_Sessions_InvalidLock:
			return "EOS_Sessions_InvalidLock";
		case EOS_EResult::EOS_Sessions_InvalidSession:
			return "EOS_Sessions_InvalidSession";
		case EOS_EResult::EOS_Sessions_SandboxNotAllowed:
			return "EOS_Sessions_SandboxNotAllowed";
		case EOS_EResult::EOS_Sessions_InviteFailed:
			return "EOS_Sessions_InviteFailed";
		case EOS_EResult::EOS_Sessions_InviteNotFound:
			return "EOS_Sessions_InviteNotFound";
		case EOS_EResult::EOS_Sessions_UpsertNotAllowed:
			return "EOS_Sessions_UpsertNotAllowed";
		case EOS_EResult::EOS_Sessions_AggregationFailed:
			return "EOS_Sessions_AggregationFailed";
		case EOS_EResult::EOS_Sessions_HostAtCapacity:
			return "EOS_Sessions_HostAtCapacity";
		case EOS_EResult::EOS_Sessions_SandboxAtCapacity:
			return "EOS_Sessions_SandboxAtCapacity";
		case EOS_EResult::EOS_Sessions_SessionNotAnonymous:
			return "EOS_Sessions_SessionNotAnonymous";
		case EOS_EResult::EOS_Sessions_OutOfSync:
			return "EOS_Sessions_OutOfSync";
		case EOS_EResult::EOS_Sessions_TooManyInvites:
			return "EOS_Sessions_TooManyInvites";
		case EOS_EResult::EOS_Sessions_PresenceSessionExists:
			return "EOS_Sessions_PresenceSessionExists";
		case EOS_EResult::EOS_Sessions_DeploymentAtCapacity:
			return "EOS_Sessions_DeploymentAtCapacity";
		case EOS_EResult::EOS_Sessions_NotAllowed:
			return "EOS_Sessions_NotAllowed";

		case EOS_EResult::EOS_PlayerDataStorage_FilenameInvalid:
			return "EOS_PlayerDataStorage_FilenameInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_FilenameLengthInvalid:
			return "EOS_PlayerDataStorage_FilenameLengthInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_FilenameInvalidChars:
			return "EOS_PlayerDataStorage_FilenameInvalidChars";
		case EOS_EResult::EOS_PlayerDataStorage_FileSizeTooLarge:
			return "EOS_PlayerDataStorage_FileSizeTooLarge";
		case EOS_EResult::EOS_PlayerDataStorage_FileSizeInvalid:
			return "EOS_PlayerDataStorage_FileSizeInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_FileHandleInvalid:
			return "EOS_PlayerDataStorage_FileHandleInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_DataInvalid:
			return "EOS_PlayerDataStorage_DataInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_DataLengthInvalid:
			return "EOS_PlayerDataStorage_DataLengthInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_StartIndexInvalid:
			return "EOS_PlayerDataStorage_StartIndexInvalid";
		case EOS_EResult::EOS_PlayerDataStorage_RequestInProgress:
			return "EOS_PlayerDataStorage_RequestInProgress";
		case EOS_EResult::EOS_PlayerDataStorage_UserThrottled:
			return "EOS_PlayerDataStorage_UserThrottled";
		case EOS_EResult::EOS_PlayerDataStorage_EncryptionKeyNotSet:
			return "EOS_PlayerDataStorage_EncryptionKeyNotSet";
		case EOS_EResult::EOS_PlayerDataStorage_UserErrorFromDataCallback:
			return "EOS_PlayerDataStorage_UserErrorFromDataCallback";
		case EOS_EResult::EOS_PlayerDataStorage_FileHeaderHasNewerVersion:
			return "EOS_PlayerDataStorage_FileHeaderHasNewerVersion";
		case EOS_EResult::EOS_PlayerDataStorage_FileCorrupted:
			return "EOS_PlayerDataStorage_FileCorrupted";

		case EOS_EResult::EOS_Connect_ExternalTokenValidationFailed:
			return "EOS_Connect_ExternalTokenValidationFailed";
		case EOS_EResult::EOS_Connect_UserAlreadyExists:
			return "EOS_Connect_UserAlreadyExists";
		case EOS_EResult::EOS_Connect_AuthExpired:
			return "EOS_Connect_AuthExpired";
		case EOS_EResult::EOS_Connect_InvalidToken:
			return "EOS_Connect_InvalidToken";
		case EOS_EResult::EOS_Connect_UnsupportedTokenType:
			return "EOS_Connect_UnsupportedTokenType";
		case EOS_EResult::EOS_Connect_LinkAccountFailed:
			return "EOS_Connect_LinkAccountFailed";
		case EOS_EResult::EOS_Connect_ExternalServiceUnavailable:
			return "EOS_Connect_ExternalServiceUnavailable";
		case EOS_EResult::EOS_Connect_ExternalServiceConfigurationFailure:
			return "EOS_Connect_ExternalServiceConfigurationFailure";
		case EOS_EResult::EOS_Connect_LinkAccountFailedMissingNintendoIdAccount:
			return "EOS_Connect_LinkAccountFailedMissingNintendoIdAccount";

		case EOS_EResult::EOS_UI_SocialOverlayLoadError:
			return "EOS_UI_SocialOverlayLoadError";

		case EOS_EResult::EOS_Lobby_NotOwner:
			return "EOS_Lobby_NotOwner";
		case EOS_EResult::EOS_Lobby_InvalidLock:
			return "EOS_Lobby_InvalidLock";
		case EOS_EResult::EOS_Lobby_LobbyAlreadyExists:
			return "EOS_Lobby_LobbyAlreadyExists";
		case EOS_EResult::EOS_Lobby_SessionInProgress:
			return "EOS_Lobby_SessionInProgress";
		case EOS_EResult::EOS_Lobby_TooManyPlayers:
			return "EOS_Lobby_TooManyPlayers";
		case EOS_EResult::EOS_Lobby_NoPermission:
			return "EOS_Lobby_NoPermission";
		case EOS_EResult::EOS_Lobby_InvalidSession:
			return "EOS_Lobby_InvalidSession";
		case EOS_EResult::EOS_Lobby_SandboxNotAllowed:
			return "EOS_Lobby_SandboxNotAllowed";
		case EOS_EResult::EOS_Lobby_InviteFailed:
			return "EOS_Lobby_InviteFailed";
		case EOS_EResult::EOS_Lobby_InviteNotFound:
			return "EOS_Lobby_InviteNotFound";
		case EOS_EResult::EOS_Lobby_UpsertNotAllowed:
			return "EOS_Lobby_UpsertNotAllowed";
		case EOS_EResult::EOS_Lobby_AggregationFailed:
			return "EOS_Lobby_AggregationFailed";
		case EOS_EResult::EOS_Lobby_HostAtCapacity:
			return "EOS_Lobby_HostAtCapacity";
		case EOS_EResult::EOS_Lobby_SandboxAtCapacity:
			return "EOS_Lobby_SandboxAtCapacity";
		case EOS_EResult::EOS_Lobby_TooManyInvites:
			return "EOS_Lobby_TooManyInvites";
		case EOS_EResult::EOS_Lobby_DeploymentAtCapacity:
			return "EOS_Lobby_DeploymentAtCapacity";
		case EOS_EResult::EOS_Lobby_NotAllowed:
			return "EOS_Lobby_NotAllowed";
		case EOS_EResult::EOS_Lobby_MemberUpdateOnly:
			return "EOS_Lobby_MemberUpdateOnly";


		case EOS_EResult::EOS_UnexpectedError:
			return "EOS_UnexpectedError";
	}

	return "Unknown";
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
