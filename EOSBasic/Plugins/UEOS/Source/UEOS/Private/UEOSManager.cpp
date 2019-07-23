// Copyright (C) Gaslight Games Ltd, 2019

#include "UEOSManager.h"

#include "Config/UEOSConfig.h"
#include "Authentication/Authentication.h"
#include "Metrics/Metrics.h"

#include "UEOSModule.h"
#include <string>

// Static Initialization
UEOSManager* UEOSManager::EOSManager = nullptr;

UEOSManager::UEOSManager()
	: PlatformHandle( NULL )
	, bEOSInitialized( false )
	, Authentication( nullptr )
	, Metrics( nullptr )
{
	
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

EOS_HPlatform UEOSManager::GetPlatformHandle()
{
	return UEOSManager::EOSManager->PlatformHandle;
}

bool UEOSManager::InitEOS()
{
	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	std::string ProductName = "UEOS Plugin";
	std::string ProductVersion = "0.2";

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

	EOS_EResult InitResult = EOS_Initialize( &SDKOptions );
	FString MessageText;

	if( InitResult != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Init Failed." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
	}

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Init Success!" ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	// Create platform instance
	EOS_Platform_Options PlatformOptions;
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;

	if( EOSConfig != nullptr )
	{
		PlatformOptions.bIsServer = ( EOSConfig->bIsServer ) ? EOS_TRUE : EOS_FALSE;

		std::string ProductId = TCHAR_TO_UTF8( *EOSConfig->ProductId );
		std::string SandboxId = TCHAR_TO_UTF8( *EOSConfig->SandboxId );

		PlatformOptions.ProductId = ProductId.c_str();
		PlatformOptions.SandboxId = SandboxId.c_str();
		
		std::string ClientId = TCHAR_TO_UTF8( *EOSConfig->ClientId );
		std::string ClientSecret = TCHAR_TO_UTF8( *EOSConfig->ClientSecret );

		PlatformOptions.ClientCredentials.ClientId = ( EOSConfig->ClientId.IsEmpty() ) ? nullptr : ClientId.c_str();
		PlatformOptions.ClientCredentials.ClientSecret = ( EOSConfig->ClientSecret.IsEmpty() ) ? nullptr : ClientSecret.c_str();

		if( EOSConfig->bAllowReservedPlatforms )
		{
			// Taken from the Sample, but this function and it's include could not be found.
			// Left for posterity, but defaulting to NULL (as is used in the Epic sample).
			//SetReservedPlatformOptions( PlatformOptions );
			PlatformOptions.Reserved = NULL;
		}
		else
		{
			PlatformOptions.Reserved = NULL;
		}
		
		PlatformHandle = EOS_Platform_Create( &PlatformOptions );
	}

	if( PlatformHandle == nullptr )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Platform Create - Failed!" ) );
		UE_LOG( UEOSLog, Warning, TEXT("%s"), *MessageText );

		return false;
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

		return false;
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
	return true;
}

bool UEOSManager::ShutdownEOS()
{
	if( PlatformHandle != nullptr )
	{
		EOS_Platform_Release( PlatformHandle );
	}

	EOS_EResult ShutdownResult = EOS_Shutdown();

	FString MessageText;
	bool bShutdownSuccess = false;
	if( ShutdownResult != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Shutdown Failed." ) );
	}
	else
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Shutdown Complete." ) );
		bShutdownSuccess = true;
		bEOSInitialized = false;
	}

	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	return bShutdownSuccess;
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
		case EOS_EResult::EOS_MissingParameters:
			return "EOS_MissingParameters";
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
		case EOS_EResult::EOS_Auth_DeviceAuth_AccountNotActive:
			return "EOS_Auth_DeviceAuth_AccountNotActive";
		case EOS_EResult::EOS_Auth_DeviceAuth_InvalidInfo:
			return "EOS_Auth_DeviceAuth_InvalidInfo";
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
