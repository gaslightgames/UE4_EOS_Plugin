// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// Engine Includes
#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "Interfaces/OnlineIdentityInterface.h"

// EOS Subsystem Includes
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEOSTypes.h"

// EOS SDK Includes
#include "eos_sdk.h"
#include "eos_auth.h"

// Forward Declarations
class FOnlineSubsystemEOS;



class FOnlineIdentityEOS : public IOnlineIdentity
{

public:

	virtual ~FOnlineIdentityEOS() {};

	// IOnlineIdentity

	virtual bool									Login( int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials ) override;
	virtual bool									Logout( int32 LocalUserNum ) override;
	virtual bool									AutoLogin( int32 LocalUserNum ) override;
	virtual TSharedPtr<FUserOnlineAccount>			GetUserAccount( const FUniqueNetId& UserId ) const override;
	virtual TArray<TSharedPtr<FUserOnlineAccount>>	GetAllUserAccounts() const override;
	virtual TSharedPtr<const FUniqueNetId>			GetUniquePlayerId( int32 LocalUserNum ) const override;
	virtual TSharedPtr<const FUniqueNetId>			CreateUniquePlayerId( uint8* Bytes, int32 Size ) override;
	virtual TSharedPtr<const FUniqueNetId>			CreateUniquePlayerId( const FString& Str ) override;
	virtual ELoginStatus::Type						GetLoginStatus( int32 LocalUserNum ) const override;
	virtual ELoginStatus::Type						GetLoginStatus( const FUniqueNetId& UserId ) const override;
	virtual FString									GetPlayerNickname( int32 LocalUserNum ) const override;
	virtual FString									GetPlayerNickname( const FUniqueNetId& UserId ) const override;
	virtual FString									GetAuthToken( int32 LocalUserNum ) const override;
	virtual void									RevokeAuthToken( const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate ) override;
	virtual void									GetUserPrivilege( const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate ) override;
	virtual FPlatformUserId							GetPlatformUserIdFromUniqueNetId( const FUniqueNetId& UniqueNetId ) const override;
	virtual FString									GetAuthType() const override;

protected:

	static void										LoginCompleteCallback( const EOS_Auth_LoginCallbackInfo* Data );

	static void										LogoutCompleteCallback( const EOS_Auth_LogoutCallbackInfo* Data );

	/** The Epic Account ID for this Authentication session. */
	FUniqueNetIdEOS									EpicAccountId;

private:

PACKAGE_SCOPE :
		
	FOnlineIdentityEOS( FOnlineSubsystemEOS* InSubsystem );

	/** The steam user interface to use when interacting with steam */
	//class ISteamUser* SteamUserPtr;
	/** The steam friends interface to use when interacting with steam */
	//class ISteamFriends* SteamFriendsPtr;

	/** Cached pointer to owning subsystem */
	FOnlineSubsystemEOS*							EOSSubsystem;
};

typedef TSharedPtr<FOnlineIdentityEOS, ESPMode::ThreadSafe> FOnlineIdentityEOSPtr;