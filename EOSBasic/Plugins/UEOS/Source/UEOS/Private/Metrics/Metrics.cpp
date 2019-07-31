// Copyright (C) Gaslight Games Ltd, 2019

#include "Metrics.h"

// EOS Plugin Includes
#include "UEOSModule.h"
#include "UEOSManager.h"

#include <string>

UEOSMetrics::UEOSMetrics()
	: AccountType( ESupportedAccountType::SAT_Unknown )
	, MetricsHandle( NULL )
{

}

bool UEOSMetrics::InitMetric()
{
	if( UEOSManager::GetPlatformHandle() != nullptr )
	{
		MetricsHandle = EOS_Platform_GetMetricsInterface( UEOSManager::GetPlatformHandle() );

		return true;
	}

	FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Metrics Init Failed - no Platform Handle.  Have you initialized the SDK?" ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	return false;
}

bool UEOSMetrics::BeginPlayerSession( FString DisplayName, ESupportedAccountType TargetAccountType, EInputControllerType ControllerType )
{
	FString MessageText;

	if( MetricsHandle == nullptr )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Cannot BeginPlayerSession - no Platform Handle.  Have you initialized the Metrics?" ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
	}

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Begin Player Session..." ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	std::string MetricsDisplayName = TCHAR_TO_UTF8( *DisplayName );

	// Begin Player Session Metrics
	EOS_Metrics_BeginPlayerSessionOptions MetricsOptions = {};
	MetricsOptions.ApiVersion = EOS_METRICS_BEGINPLAYERSESSION_API_LATEST;

	switch( TargetAccountType )
	{
	case ESupportedAccountType::SAT_Epic:
		MetricsOptions.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_Epic;
		// TODO: Get this from the Manager, then from the Authentication.  Once we can confirm this works.
		//MetricsOptions.AccountId.Epic = FSFData::Get().GetLocalUserID();
		break;
	case ESupportedAccountType::SAT_External:
		MetricsOptions.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_External;
		MetricsOptions.AccountId.External = TCHAR_TO_UTF8( *DisplayName );
		break;
	case ESupportedAccountType::SAT_Unknown:
	default:
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Unknown or unsupported Account Type." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
		return false;
		break;
	}
	
	AccountType = TargetAccountType;

	MetricsOptions.DisplayName = MetricsDisplayName.c_str();
	MetricsOptions.ServerIp = "127.0.0.1";						// TODO: Retrieve this from the Networking layer.
	MetricsOptions.GameSessionId = "";							// TODO: Properly expose this as an option for starting the session!

	switch( ControllerType )
	{
	case EInputControllerType::ICT_MouseKeyboard:
		MetricsOptions.ControllerType = EOS_EUserControllerType::EOS_UCT_MouseKeyboard;
		break;
	case EInputControllerType::ICT_Gamepad:
		MetricsOptions.ControllerType = EOS_EUserControllerType::EOS_UCT_GamepadControl;
		break;
	case EInputControllerType::ICT_Touch:
		MetricsOptions.ControllerType = EOS_EUserControllerType::EOS_UCT_TouchControl;
		break;
	case EInputControllerType::ICT_Unknown:
	default:
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Unknown or unsupported Input Controller Type." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
		break;
	}
	
	MetricsOptions.ServerIp = nullptr;
	MetricsOptions.GameSessionId = nullptr;

	const EOS_EResult Result = EOS_Metrics_BeginPlayerSession( MetricsHandle, &MetricsOptions );
	if( Result != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Begin Player Session Failed! Result: %s." ), *UEOSManager::EOSResultToString( Result ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
	}

	return true;
}

bool UEOSMetrics::EndPlayerSession( FString DisplayName )
{
	FString MessageText;

	if( MetricsHandle == nullptr )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Cannot EndPlayerSession - no Metrics Handle." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
	}

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] End Player Session..." ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	// End Player Session Metrics
	EOS_Metrics_EndPlayerSessionOptions MetricsOptions = {};
	MetricsOptions.ApiVersion = EOS_METRICS_ENDPLAYERSESSION_API_LATEST;

	switch( AccountType )
	{
	case ESupportedAccountType::SAT_Epic:
		MetricsOptions.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_Epic;
		// TODO: Get this from the Manager, then from the Authentication.  Once we can confirm this works.
		//MetricsOptions.AccountId.Epic = FSFData::Get().GetLocalUserID();
		break;
	case ESupportedAccountType::SAT_External:
		MetricsOptions.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_External;
		MetricsOptions.AccountId.External = TCHAR_TO_UTF8( *DisplayName );
		break;
	case ESupportedAccountType::SAT_Unknown:
	default:
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Unknown or unsupported Account Type." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
		return false;
		break;
	}

	const EOS_EResult Result = EOS_Metrics_EndPlayerSession( MetricsHandle, &MetricsOptions );
	if( Result != EOS_EResult::EOS_Success )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] End Player Session Failed! Result: %s." ), *UEOSManager::EOSResultToString( Result ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		return false;
	}

	return true;
}
