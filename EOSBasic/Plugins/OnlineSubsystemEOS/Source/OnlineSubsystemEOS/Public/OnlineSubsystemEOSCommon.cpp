// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "OnlineSubsystemEOSCommon.h"


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
	case EOS_EResult::EOS_MissingParameters_DEPRECATED:
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

FString UEOSCommon::EOSResultToString( EOS_EResult Result )
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
