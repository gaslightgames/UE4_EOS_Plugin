// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "Interfaces/OnlineIdentityInterface.h"

#include "OnlineSubsystemEOS.h"

// EOS Includes
#include "eos_sdk.h"
#include "eos_auth.h"

class FOnlineSubsystemEOS;


/**
 * Epic Online Services specific implementation of the unique net id
 */
class FUniqueNetIdEOS :	public FUniqueNetId
{

PACKAGE_SCOPE:

	/** The EOS SDK matching Account Id. */
	EOS_EpicAccountId								EpicAccountId;

	/** Hidden on purpose */
	FUniqueNetIdEOS() :
		EpicAccountId()
	{
	}

	/**
	 * Copy Constructor
	 *
	 * @param Src the id to copy
	 */
	explicit FUniqueNetIdEOS( const FUniqueNetIdEOS& Src )
		: EpicAccountId( Src.EpicAccountId )
	{
	}

public:

	explicit FUniqueNetIdEOS( EOS_EpicAccountId InAccountId )
		: EpicAccountId( InAccountId )
	{
	}

	/**
	 * Constructs this object with the specified net id
	 *
	 * @param String textual representation of an id
	 */
	explicit FUniqueNetIdEOS( const FString& Str ) :
		EpicAccountId( FUniqueNetIdEOS::FromString( *Str ) )
	{
	}

	virtual FName GetType() const override
	{
		return EOS_SUBSYSTEM;
	}

	/**
	 * Get the raw byte representation of this net id
	 * This data is platform dependent and shouldn't be manipulated directly
	 *
	 * @return byte array of size GetSize()
	 */
	virtual const uint8* GetBytes() const override
	{
		return (uint8*)&EpicAccountId;
	}

	/**
	 * Get the size of the id
	 *
	 * @return size in bytes of the id representation
	 */
	virtual int32 GetSize() const override
	{
		return sizeof( EOS_EpicAccountId );
	}

	/**
	 * Check the validity of the id
	 *
	 * @return true if this is a well formed ID, false otherwise
	 */
	virtual bool IsValid() const override
	{
		return ( EOS_EpicAccountId_IsValid( EpicAccountId ) == EOS_TRUE ) ? true : false;
	}

	/**
	 * Platform specific conversion to string representation of data
	 *
	 * @return data in string form
	 */
	virtual FString ToString() const override
	{
		static char TempBuffer[EOS_EPICACCOUNTID_MAX_LENGTH];
		int32_t TempBufferSize = sizeof( TempBuffer );
		EOS_EpicAccountId_ToString( EpicAccountId, TempBuffer, &TempBufferSize );
		FString returnValue( TempBuffer );
		return returnValue;
	}

	static EOS_EpicAccountId FromString( const FString& AccountId )
	{
		return EOS_EpicAccountId_FromString( TCHAR_TO_ANSI( *AccountId ) );
	}

	/**
	 * Get a human readable representation of the net id
	 * Shouldn't be used for anything other than logging/debugging
	 *
	 * @return id in string form
	 */
	virtual FString ToDebugString() const override
	{
		if( IsValid() == true )
		{
			return ToString();
		}
		
		return TEXT( "INVALID" );
	}

	/** Needed for TMap::GetTypeHash() */
	friend uint32 GetTypeHash( const FUniqueNetIdEOS& A )
	{
		return GetTypeHash( A.ToString() );
	}

	/** global static instance of invalid (zero) id */
	static const TSharedRef<const FUniqueNetId>& EmptyId()
	{
		static const TSharedRef<const FUniqueNetId> EmptyId( MakeShared<FUniqueNetIdEOS>() );
		return EmptyId;
	}

	/** Convenience cast to EOS_EpicAccountId */
	operator EOS_EpicAccountId()
	{
		return EpicAccountId;
	}

	/** Convenience cast to EOS_EpicAccountId */
	operator const EOS_EpicAccountId() const
	{
		return EpicAccountId;
	}

	/** Convenience cast to EOS_EpicAccountId pointer */
	operator EOS_EpicAccountId*( )
	{
		return (EOS_EpicAccountId*)&EpicAccountId;
	}

	/** Convenience cast to EOS_EpicAccountId pointer */
	operator const EOS_EpicAccountId*( ) const
	{
		return (const EOS_EpicAccountId*)&EpicAccountId;
	}

	//friend FArchive& operator<<( FArchive& Ar, FUniqueNetIdEOS& UserId )
	//{
	//	return Ar << UserId.ToString();
	//}
};


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