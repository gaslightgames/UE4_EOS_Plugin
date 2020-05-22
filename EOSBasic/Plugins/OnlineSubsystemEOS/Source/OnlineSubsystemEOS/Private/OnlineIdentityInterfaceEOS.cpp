// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "OnlineIdentityInterfaceEOS.h"
#include "OnlineSubsystemEOS.h"
//#include "OnlineSubsystemSteamTypes.h"
#include "OnlineError.h"

FOnlineIdentityEOS::FOnlineIdentityEOS( FOnlineSubsystemEOS* InSubsystem )
	: EOSSubsystem( InSubsystem )
{
	
}

bool FOnlineIdentityEOS::Login( int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials )
{
	return false;
}

bool FOnlineIdentityEOS::Logout( int32 LocalUserNum )
{
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
