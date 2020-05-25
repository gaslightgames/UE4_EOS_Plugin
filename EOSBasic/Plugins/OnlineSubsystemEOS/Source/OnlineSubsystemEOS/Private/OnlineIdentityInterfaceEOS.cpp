// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "OnlineIdentityInterfaceEOS.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEOSCommon.h"
// @todo: create helper classes/functions for converting between more BP/dev friendly types
//			to more generic elements for the OSS. Such as EOS Login Mode(s).
//#include "OnlineSubsystemSteamTypes.h"
#include "OnlineError.h"

#include <string>

FOnlineIdentityEOS::FOnlineIdentityEOS( FOnlineSubsystemEOS* InSubsystem )
	: EpicAccountId()
	, EOSSubsystem( InSubsystem )
{
	
}

bool FOnlineIdentityEOS::Login( int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials )
{
	FString ErrorStr;
	if( LocalUserNum < MAX_LOCAL_PLAYERS )
	{
		if( EOSSubsystem->IsEOSInitialized() == true )
		{
			EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface( EOSSubsystem->GetPlatformHandle() );

			if( AuthHandle != nullptr )
			{
				EOS_Auth_Credentials Credentials;
				Credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;

				EOS_Auth_LoginOptions LoginOptions;
				memset( &LoginOptions, 0, sizeof( LoginOptions ) );
				LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;

				// Use AccountCredentials.Type then:
				// @todo: support ALL Login types
				FString MessageText = FString::Printf( TEXT( "Logging In with Dev Auth Tool | ID: %s | Token: %s." ), *AccountCredentials.Id, *AccountCredentials.Token );
				UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );

				Credentials.Id = TCHAR_TO_UTF8( *AccountCredentials.Id );
				Credentials.Token = TCHAR_TO_UTF8( *AccountCredentials.Token );
				Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Developer;

				LoginOptions.Credentials = &Credentials;

				EOS_Auth_Login( AuthHandle, &LoginOptions, NULL, LoginCompleteCallback );

				return true;
			}
			else
			{
				ErrorStr = TEXT( "Failed to get AuthHandle." );
			}
		}
		else
		{
			ErrorStr = TEXT( "EOS SDK Is not Initialized." );
		}
	}
	else
	{
		// Requesting a local user is always invalid
		ErrorStr = FString::Printf( TEXT( "Invalid user %d" ), LocalUserNum );
	}

	if( !ErrorStr.IsEmpty() )
	{
		UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "Failed Epic Online Services login. %s" ), *ErrorStr );
		TriggerOnLoginCompleteDelegates( LocalUserNum, false, FUniqueNetIdEOS(), ErrorStr );
	}

	return false;
}

bool FOnlineIdentityEOS::Logout( int32 LocalUserNum )
{
	FString ErrorStr;
	if( EOSSubsystem->IsEOSInitialized() == true )
	{
		EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface( EOSSubsystem->GetPlatformHandle() );

		if( AuthHandle != nullptr )
		{
			EOS_Auth_LogoutOptions LogoutOptions;
			LogoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
			LogoutOptions.LocalUserId = EpicAccountId;

			EOS_Auth_Logout( AuthHandle, &LogoutOptions, NULL, LogoutCompleteCallback );

			return true;
		}
		else
		{
			ErrorStr = TEXT( "Failed to get AuthHandle." );
		}
	}
	else
	{
		ErrorStr = TEXT( "EOS SDK Is not Initialized." );
	}

	if( !ErrorStr.IsEmpty() )
	{
		UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "Failed Epic Online Services logout. %s" ), *ErrorStr );
		TriggerOnLoginCompleteDelegates( LocalUserNum, false, FUniqueNetIdEOS(), ErrorStr );
	}

	return false;
}

bool FOnlineIdentityEOS::AutoLogin( int32 LocalUserNum )
{
	return false;
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityEOS::GetUserAccount( const FUniqueNetId& UserId ) const
{
	//@todo - not implemented
	return nullptr;
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityEOS::GetAllUserAccounts() const
{
	//@todo - not implemented
	return TArray<TSharedPtr<FUserOnlineAccount> >();
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEOS::GetUniquePlayerId( int32 LocalUserNum ) const
{
	return NULL;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEOS::CreateUniquePlayerId( uint8* Bytes, int32 Size )
{
	return NULL;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEOS::CreateUniquePlayerId( const FString& Str )
{
	return NULL;
}

ELoginStatus::Type FOnlineIdentityEOS::GetLoginStatus( int32 LocalUserNum ) const
{
	return ELoginStatus::Type::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityEOS::GetLoginStatus( const FUniqueNetId& UserId ) const
{
	return ELoginStatus::Type::NotLoggedIn;
}

FString FOnlineIdentityEOS::GetPlayerNickname( int32 LocalUserNum ) const
{
	return "";
}

FString FOnlineIdentityEOS::GetPlayerNickname( const FUniqueNetId& UserId ) const
{
	return "";
}

FString FOnlineIdentityEOS::GetAuthToken( int32 LocalUserNum ) const
{
	return "";
}

void FOnlineIdentityEOS::RevokeAuthToken( const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate )
{

}

void FOnlineIdentityEOS::GetUserPrivilege( const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate )
{

}

FPlatformUserId FOnlineIdentityEOS::GetPlatformUserIdFromUniqueNetId( const FUniqueNetId& UniqueNetId ) const
{
	return FPlatformUserId();
}

FString FOnlineIdentityEOS::GetAuthType() const
{
	return "";
}

void FOnlineIdentityEOS::LoginCompleteCallback( const EOS_Auth_LoginCallbackInfo* Data )
{
	check( Data != NULL );

	FUniqueNetIdEOS EpicId( Data->LocalUserId );
	FString MessageText = FString::Printf( TEXT( "EOS Login Complete - User ID: %s" ), *EpicId.ToString() );
	UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get( EOS_SUBSYSTEM );
	FOnlineSubsystemEOS* EOSSubsystem = (FOnlineSubsystemEOS*)SubSystem;
	
	if( EOSSubsystem != nullptr )
	{
		IOnlineIdentityPtr OnlineIdentity = EOSSubsystem->GetIdentityInterface();

		if( OnlineIdentity != nullptr )
		{
			EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface( EOSSubsystem->GetPlatformHandle() );

			if( AuthHandle != nullptr )
			{
				if( Data->ResultCode == EOS_EResult::EOS_Success )
				{
					const int32_t AccountsCount = EOS_Auth_GetLoggedInAccountsCount( AuthHandle );
					for( int32_t AccountIdx = 0; AccountIdx < AccountsCount; ++AccountIdx )
					{
						FUniqueNetIdEOS EpicAccountId( EOS_Auth_GetLoggedInAccountByIndex( AuthHandle, AccountIdx ) );

						EOS_ELoginStatus LoginStatus;
						LoginStatus = EOS_Auth_GetLoginStatus( AuthHandle, EpicAccountId.EpicAccountId );

						MessageText = FString::Printf( TEXT( "EOS Login: AccountIdx: %d Status: %d" ), AccountIdx, (int32_t)LoginStatus );
						UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );
					}

					// @todo: Match the returned User with some Local User Num.
					OnlineIdentity->TriggerOnLoginChangedDelegates( 0 );
					// @todo: Match the returned User with some Local User Num.
					OnlineIdentity->TriggerOnLoginCompleteDelegates( 0, true, EpicId, TEXT( "" ) );
				}
				else
				{
					MessageText = FString::Printf( TEXT( "EOS Login: Failed with Status: %s" ), *UEOSCommon::EOSResultToString( Data->ResultCode ) );
					UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );
					// @todo: Match the returned User with some Local User Num.
					int32 LocalUserNum = 0;
					OnlineIdentity->TriggerOnLoginCompleteDelegates( LocalUserNum, false, FUniqueNetIdEOS(), MessageText );
				}
			}
			else
			{
				MessageText = FString::Printf( TEXT( "EOS Login: Failed to retrieve EOS Auth Handle." ) );
				UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );

				// @todo: Match the returned User with some Local User Num.
				int32 LocalUserNum = 0;
				OnlineIdentity->TriggerOnLoginCompleteDelegates( LocalUserNum, false, FUniqueNetIdEOS(), MessageText );
			}
		}
		else
		{
			MessageText = FString::Printf( TEXT( "EOS Login: Failed to retrieve EOS Online Identity Interface." ) );
			UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );
		}
	}
	else
	{
		MessageText = FString::Printf( TEXT( "EOS Login: Cannot retrieve EOS Subsystem." ) );
		UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );
	}
}

void FOnlineIdentityEOS::LogoutCompleteCallback( const EOS_Auth_LogoutCallbackInfo* Data )
{
	check( Data != NULL );

	FString MessageText = FString::Printf( TEXT( "EOS Logout Complete." ) );
	UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get( EOS_SUBSYSTEM );

	if( SubSystem != nullptr )
	{
		IOnlineIdentityPtr OnlineIdentity = SubSystem->GetIdentityInterface();

		if( OnlineIdentity != nullptr )
		{
			OnlineIdentity->TriggerOnLogoutCompleteDelegates( 0, false );
		}
	}
	else
	{
		MessageText = FString::Printf( TEXT( "EOS Login: Failed to retrieve EOS Online Identity Interface." ) );
		UE_LOG_ONLINE_IDENTITY( Warning, TEXT( "%s" ), *MessageText );
	}
}
