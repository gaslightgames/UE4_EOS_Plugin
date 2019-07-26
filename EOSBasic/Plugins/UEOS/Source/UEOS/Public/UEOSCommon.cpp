// Copyright (C) Gaslight Games Ltd, 2019

#include "UEOSCommon.h"

EEOSResultType UEOSCommon::GetUnrealFriendlyResult( EOS_EResult SDKResult, EEOSResults& Result, EEOSAuth& Auth, EEOSFriends& Friends, EEOSPresence& Presence, EEOSEcom& Ecom )
{
	Result = EEOSResults::ER_UnknownError;
	Auth = EEOSAuth::EA_UnknownError;
	Friends = EEOSFriends::EF_UnknownError;
	Presence = EEOSPresence::EP_UnknownError;
	Ecom = EEOSEcom::EE_UnknownError;

	// Early exit on unknown error.
	if( (int32)SDKResult == 0x7FFFFFFF )
	{
		return EEOSResultType::RT_Unknown;
	}

	if( (int32)SDKResult < 1000 )
	{
		Result = GetResultsValue( SDKResult );
		return EEOSResultType::RT_Result;
	}
	else if( (int32)SDKResult < 2000 )
	{
		// Auth...
	}
	else if( (int32)SDKResult < 3000 )
	{
		// Friends...
	}
	else if( (int32)SDKResult < 4000 )
	{
		// Presence...
	}
	else
	{
		// Ecom
	}

	return EEOSResultType::RT_Unknown;
}

EEOSResults UEOSCommon::GetResultsValue( EOS_EResult SDKResult )
{
	switch( SDKResult )
	{
	case EOS_EResult::EOS_Success:
		return EEOSResults::ER_Success;
		break;
	case EOS_EResult::EOS_NoConnection:
		return EEOSResults::ER_NoConnection;
		break;
	case EOS_EResult::EOS_InvalidCredentials:
		return EEOSResults::ER_InvalidCredentials;
		break;
	case EOS_EResult::EOS_InvalidUser:
		return EEOSResults::ER_InvalidUser;
		break;
	case EOS_EResult::EOS_InvalidAuth:
		return EEOSResults::ER_InvalidAuth;
		break;
	case EOS_EResult::EOS_AccessDenied:
		return EEOSResults::ER_AccessDenied;
		break;
	case EOS_EResult::EOS_MissingPermissions:
		return EEOSResults::ER_MissingPermissions;
		break;
	case EOS_EResult::EOS_Token_Not_Account:
		return EEOSResults::ER_TokenNotAccount;
		break;
	case EOS_EResult::EOS_TooManyRequests:
		return EEOSResults::ER_TooManyRequests;
		break;
	case EOS_EResult::EOS_AlreadyPending:
		return EEOSResults::ER_AlreadyPending;
		break;
	case EOS_EResult::EOS_InvalidParameters:
		return EEOSResults::ER_InvalidParameters;
		break;
	case EOS_EResult::EOS_InvalidRequest:
		return EEOSResults::ER_InvalidRequest;
		break;
	case EOS_EResult::EOS_UnrecognizedResponse:
		return EEOSResults::ER_UnrecognizedResponse;
		break;
	case EOS_EResult::EOS_IncompatibleVersion:
		return EEOSResults::ER_IncompatibleVersion;
		break;
	case EOS_EResult::EOS_NotConfigured:
		return EEOSResults::ER_NotConfigured;
		break;
	case EOS_EResult::EOS_AlreadyConfigured:
		return EEOSResults::ER_AlreadyConfigured;
		break;
	case EOS_EResult::EOS_NotImplemented:
		return EEOSResults::ER_NotImplemented;
		break;
	case EOS_EResult::EOS_Canceled:
		return EEOSResults::ER_Canceled;
		break;
	case EOS_EResult::EOS_NotFound:
		return EEOSResults::ER_NotFound;
		break;
	case EOS_EResult::EOS_OperationWillRetry:
		return EEOSResults::ER_OperationWillRetry;
		break;
	case EOS_EResult::EOS_NoChange:
		return EEOSResults::ER_NoChange;
		break;
	case EOS_EResult::EOS_VersionMismatch:
		return EEOSResults::ER_VersionMismatch;
		break;
	case EOS_EResult::EOS_LimitExceeded:
		return EEOSResults::ER_LimitExceeded;
		break;
	case EOS_EResult::EOS_Disabled:
		return EEOSResults::ER_Disabled;
		break;
	case EOS_EResult::EOS_DuplicateNotAllowed:
		return EEOSResults::ER_DuplicateNotAllowed;
		break;
	case EOS_EResult::EOS_MissingParameters:
		return EEOSResults::ER_MissingParameters;
		break;
	}

	return EEOSResults::ER_UnknownError;
}

EEOSAuth UEOSCommon::GetAuthValue( EOS_EResult SDKResult )
{
	return EEOSAuth::EA_UnknownError;
}

EEOSFriends UEOSCommon::GetFriendsValue( EOS_EResult SDKResult )
{
	return EEOSFriends::EF_UnknownError;
}

EEOSPresence UEOSCommon::GetPresenceValue( EOS_EResult SDKResult )
{
	return EEOSPresence::EP_UnknownError;
}

EEOSEcom UEOSCommon::GetEcomValue( EOS_EResult SDKResult )
{
	return EEOSEcom::EE_UnknownError;
}
