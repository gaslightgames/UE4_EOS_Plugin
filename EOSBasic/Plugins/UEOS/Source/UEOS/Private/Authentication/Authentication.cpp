// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "Authentication/Authentication.h"

#include "UEOSModule.h"
#include "UEOSManager.h"

#include <string>

UEOSAuthentication::UEOSAuthentication()
	: AuthHandle( NULL )
	, EpicAccountId()
	, bAuthorised( false )
{

}

void UEOSAuthentication::Login( ELoginMode LoginMode, FString UserId, FString UserToken )
{
	FString	MessageText;

	if( !UEOSManager::IsEOSInitialized() )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Can't Log In - EOS SDK Not Initialized!." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
		return;
	}

	AuthHandle = EOS_Platform_GetAuthInterface( UEOSManager::GetEOSManager()->GetPlatformHandle() );

	if( AuthHandle == nullptr )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Can't Log In - Failed to get Authentication Handle." ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
		return;
	}

	EOS_Auth_Credentials Credentials;
	Credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;

	EOS_Auth_LoginOptions LoginOptions;
	memset( &LoginOptions, 0, sizeof( LoginOptions ) );
	LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;

	switch( LoginMode )
	{
		case ELoginMode::LM_IDPassword:
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logging In as User Id: %s." ), *UserId );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
			Credentials.Id = TCHAR_TO_UTF8( *UserId );
			Credentials.Token = TCHAR_TO_UTF8( *UserToken );
			Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Password;
			break;
		}
		case ELoginMode::LM_ExchangeCode:
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logging In with Exchange Code." ) );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
			Credentials.Token = TCHAR_TO_UTF8( *UserId );
			Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExchangeCode;
			break;
		}
		case ELoginMode::LM_PinGrant:
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logging In with Pin Grant." ) );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
			Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_DeviceCode;
			break;
		}
		case ELoginMode::LM_DevTool:
		{
			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logging In with Dev Auth Tool | ID: %s | Token: %s." ), *UserId, *UserToken );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
			Credentials.Id = TCHAR_TO_UTF8( *UserId );
			Credentials.Token = TCHAR_TO_UTF8( *UserToken );
			Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Developer;

			break;
		}

		case ELoginMode::LM_AccountPortal:
		{
			MessageText = FString::Printf(TEXT("[EOS SDK | Plugin] Logging In with Account Portal Tool | ID: %s | Token: %s."), *UserId, *UserToken);
			UE_LOG(UEOSLog, Warning, TEXT("%s"), *MessageText);
			Credentials.Id = TCHAR_TO_UTF8(*UserId);
			Credentials.Token = TCHAR_TO_UTF8(*UserToken);
			Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;

			break;
		}
	}

	LoginOptions.Credentials = &Credentials;

	EOS_Auth_Login( AuthHandle, &LoginOptions, NULL, LoginCompleteCallback );

}

void UEOSAuthentication::Logout()
{
	FString	MessageText;

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logging Out..." ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	EOS_Auth_LogoutOptions LogoutOptions;
	LogoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
	LogoutOptions.LocalUserId = EpicAccountId;

	EOS_Auth_Logout( AuthHandle, &LogoutOptions, NULL, LogoutCompleteCallback );
}

bool UEOSAuthentication::GetAuthorised()
{
	return bAuthorised;
}

bool UEOSAuthentication::GetAuthTokenCopy( EOS_Auth_Token** OutToken )
{
	EOS_Auth_CopyUserAuthTokenOptions TokenOpt;
	TokenOpt.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	EOS_EResult Result = EOS_Auth_CopyUserAuthToken( AuthHandle, &TokenOpt, EpicAccountId, OutToken );
	return Result == EOS_EResult::EOS_Success;
}

void UEOSAuthentication::ReleaseAuthToken( EOS_Auth_Token* Token )
{
	EOS_Auth_Token_Release( Token );
}

FString UEOSAuthentication::AccountIDToString( EOS_EpicAccountId InAccountId )
{
	FString returnValue( "ERROR" );

	if( InAccountId == nullptr )
	{
		returnValue = "NULL";
		return returnValue;
	}

	static char TempBuffer[EOS_EPICACCOUNTID_MAX_LENGTH];
	int32_t TempBufferSize = sizeof( TempBuffer );
	EOS_EResult Result = EOS_EpicAccountId_ToString( InAccountId, TempBuffer, &TempBufferSize );

	if( Result == EOS_EResult::EOS_Success )
	{
		returnValue = TempBuffer;
		return returnValue;
	}

	FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Epic Account Id To String Error: %s" ), *UEOSCommon::EOSResultToString( Result ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	return returnValue;
}

void UEOSAuthentication::LoginCompleteCallback( const EOS_Auth_LoginCallbackInfo* Data )
{
	check( Data != NULL );

	FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Login Complete - User ID: %s" ), *AccountIDToString( Data->LocalUserId ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface( UEOSManager::GetEOSManager()->GetPlatformHandle() );

	if( Data->ResultCode == EOS_EResult::EOS_Success )
	{
		UEOSManager::GetAuthentication()->EpicAccountId = Data->LocalUserId;

		const int32_t AccountsCount = EOS_Auth_GetLoggedInAccountsCount( AuthHandle );
		for( int32_t AccountIdx = 0; AccountIdx < AccountsCount; ++AccountIdx )
		{
			FEpicAccountId AccountId;
			AccountId = EOS_Auth_GetLoggedInAccountByIndex( AuthHandle, AccountIdx );

			EOS_ELoginStatus LoginStatus;
			LoginStatus = EOS_Auth_GetLoginStatus( AuthHandle, UEOSManager::GetAuthentication()->GetEpicAccountId());

			MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] AccountIdx: %d Status: %d" ), AccountIdx, (int32_t)LoginStatus );
			UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
		}

		UEOSManager::GetAuthentication()->bAuthorised = true;

		// Broadcast the LoggedIn Delegate.
		if( UEOSManager::GetAuthentication()->OnUserLoggedIn.IsBound() )
		{
			UEOSManager::GetAuthentication()->OnUserLoggedIn.Broadcast();
		}
	}
	else if( Data->ResultCode == EOS_EResult::EOS_Auth_PinGrantCode )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Waiting for PIN Grant Code: %s URI: %s Expires: %d" ), Data->PinGrantInfo->UserCode, Data->PinGrantInfo->VerificationURI, Data->PinGrantInfo->ExpiresIn );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	}
	else if( Data->ResultCode == EOS_EResult::EOS_Auth_MFARequired )
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] MFA Code needs to be entered before logging in" ) );
		UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

		// Broadcast the Requires MFA Delegate.
		if( UEOSManager::GetAuthentication()->OnUserLoginRequiresMFA.IsBound() )
		{
			UEOSManager::GetAuthentication()->OnUserLoginRequiresMFA.Broadcast();
		}
	}
	else
	{
		MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Login Failed - Error Code: %s" ), *UEOSCommon::EOSResultToString( Data->ResultCode ) );
		UE_LOG( UEOSLog, Error, TEXT( "%s" ), *MessageText );

		// Broadcast the Login Failure Delegate.
		if( UEOSManager::GetAuthentication()->OnUserLoginFail.IsBound() )
		{
			UEOSManager::GetAuthentication()->OnUserLoginFail.Broadcast();
		}
	}
}

void UEOSAuthentication::LogoutCompleteCallback( const EOS_Auth_LogoutCallbackInfo* Data )
{
	check( Data != NULL );

	FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] Logout Complete: %s | User: %s" ), *UEOSCommon::EOSResultToString( Data->ResultCode ), *AccountIDToString( Data->LocalUserId ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	UEOSManager::GetAuthentication()->bAuthorised = true;

	// Broadcast the Logged Out Delegate.
	if( UEOSManager::GetAuthentication()->OnUserLoggedOut.IsBound() )
	{
		UEOSManager::GetAuthentication()->OnUserLoggedOut.Broadcast();
	}
}

void UEOSAuthentication::PrintAuthToken( EOS_Auth_Token* InAuthToken )
{
	FString MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] AuthToken:" ) );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );

	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] - App: %s" ), InAuthToken->App );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] - ClientId: %s" ), InAuthToken->ClientId );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] - AccountId: %s" ), InAuthToken->AccountId );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] - AccessToken: %s" ), InAuthToken->AccessToken );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] - ExpiresIn: %0.2f" ), InAuthToken->ExpiresIn );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
	MessageText = FString::Printf( TEXT( "[EOS SDK | Plugin] - ExpiresAt: %s" ), InAuthToken->ExpiresAt );
	UE_LOG( UEOSLog, Warning, TEXT( "%s" ), *MessageText );
}

FString FEpicAccountId::ToString() const
{
	static char TempBuffer[EOS_EPICACCOUNTID_MAX_LENGTH];
	int32_t TempBufferSize = sizeof( TempBuffer );
	EOS_EpicAccountId_ToString(EpicAccountId, TempBuffer, &TempBufferSize );
	FString returnValue( TempBuffer );
	return returnValue;
}

FEpicAccountId FEpicAccountId::FromString(const FString& AccountId)
{
	EOS_EpicAccountId Account = EOS_EpicAccountId_FromString(TCHAR_TO_ANSI(*AccountId));
	return FEpicAccountId(Account);
}

FEpicAccountId::operator bool() const
{
	return ( EOS_EpicAccountId_IsValid( EpicAccountId ) == EOS_TRUE ) ? true : false;
}
