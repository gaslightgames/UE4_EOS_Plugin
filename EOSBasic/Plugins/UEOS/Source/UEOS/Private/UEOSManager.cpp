// Copyright (C) Gaslight Games Ltd, 2019

#include "UEOSManager.h"

#include "Config/UEOSConfig.h"

#include "UEOSModule.h"

// Static Initialization
UEOSManager* UEOSManager::EOSManager = nullptr;

UEOSManager::UEOSManager()
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

bool UEOSManager::InitEOS()
{
	UEOSConfig* EOSConfig = GetMutableDefault<UEOSConfig>();

	// Init EOS SDK
	EOS_InitializeOptions SDKOptions;
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = nullptr;
	SDKOptions.ReallocateMemoryFunction = nullptr;
	SDKOptions.ReleaseMemoryFunction = nullptr;
	SDKOptions.ProductName = "UEOSPlugin";
	SDKOptions.ProductVersion = "0.1";

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

	FString ProductId;
	FString SandboxId;

	if( EOSConfig != nullptr )
	{
		PlatformOptions.bIsServer = ( EOSConfig->bIsServer ) ? EOS_TRUE : EOS_FALSE;

		ProductId = EOSConfig->ProductId;
		SandboxId = EOSConfig->SandboxId;

		PlatformOptions.ClientCredentials.ClientId = ( EOSConfig->ClientId.IsEmpty() ) ? nullptr : TCHAR_TO_UTF8( *EOSConfig->ClientId );
		PlatformOptions.ClientCredentials.ClientSecret = ( EOSConfig->ClientSecret.IsEmpty() ) ? nullptr : TCHAR_TO_UTF8( *EOSConfig->ClientSecret );

		if( EOSConfig->bAllowReservedPlatforms )
		{
			// Taken from the Sample, but this function and it's include could not be found.
			// Left for posterity, but defaulting to NULL.
			//SetReservedPlatformOptions( PlatformOptions );
			PlatformOptions.Reserved = NULL;
		}
		else
		{
			PlatformOptions.Reserved = NULL;
		}
	}
	else
	{
		PlatformOptions.bIsServer = EOS_FALSE;
		PlatformOptions.ClientCredentials.ClientId = nullptr;
		PlatformOptions.ClientCredentials.ClientSecret = nullptr;
		PlatformOptions.Reserved = NULL;
	}

	PlatformHandle = EOS_Platform_Create( &PlatformOptions );

	if( PlatformHandle == nullptr )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Platform Create - Failed!" ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
	}

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Platform Create - Success!" ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

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
	}

	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	return bShutdownSuccess;
}
