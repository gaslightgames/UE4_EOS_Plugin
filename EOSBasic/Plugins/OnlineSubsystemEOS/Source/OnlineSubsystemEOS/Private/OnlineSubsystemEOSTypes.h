// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// Engine Includes
#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "IPAddress.h"

// EOS Includes
#include "OnlineSubsystemEOS.h"


/** Possible session states */
namespace EEOSSession
{
	enum Type
	{
		/** Session is undefined */
		None,
		/** Session managed as a lobby on backend */
		LobbySession,
		/** Session managed by master server publishing */
		AdvertisedSessionHost,
		/** Session client of a game server session */
		AdvertisedSessionClient,
		/** Session managed by LAN beacon */
		LANSession,
	};

	/** @return the stringified version of the enum passed in */
	inline const TCHAR* ToString( EEOSSession::Type SessionType )
	{
		switch( SessionType )
		{
		case None:
		{
			return TEXT( "Session undefined" );
		}
		case LobbySession:
		{
			return TEXT( "Lobby session" );
		}
		case AdvertisedSessionHost:
		{
			return TEXT( "Advertised Session Host" );
		}
		case AdvertisedSessionClient:
		{
			return TEXT( "Advertised Session Client" );
		}
		case LANSession:
		{
			return TEXT( "LAN Session" );
		}
		}
		return TEXT( "" );
	}
}

/**
 * Epic Online Services specific implementation of the unique net id
 */
class FUniqueNetIdEOS : public FUniqueNetId
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

/** Data regarding preferred session connection methods */
enum class FEOSConnectionMethod : int8
{
	None = 0,
	Direct,
	P2P,
	PartnerHosted
};

/** Functionality for converting a connection method type between formats */
FString LexToString( const FEOSConnectionMethod Method );
FEOSConnectionMethod ToConnectionMethod( const FString& InString );

/**
 * Implementation of session information
 */
class FOnlineSessionInfoEOS : public FOnlineSessionInfo
{
protected:

	/** Hidden on purpose */
	FOnlineSessionInfoEOS( const FOnlineSessionInfoEOS& Src )
	{
	}

	/** Hidden on purpose */
	FOnlineSessionInfoEOS& operator=( const FOnlineSessionInfoEOS& Src )
	{
		return *this;
	}

PACKAGE_SCOPE:

	/** Constructor for LAN sessions */
	FOnlineSessionInfoEOS( EEOSSession::Type SessionType = EEOSSession::None );

	/** Constructor for sessions that represent a Steam lobby or an advertised server session */
	FOnlineSessionInfoEOS( EEOSSession::Type SessionType, const FUniqueNetIdEOS& InSessionId );

	/**
	 * Initialize a Steam session info with the address of this machine
	 */
	void Init();

	/**
	 * Initialize a Steam session info with the address of this machine
	 */
	void InitLAN();

	/** Type of session this is, affects interpretation of id below */
	EEOSSession::Type SessionType;
	/** The ip & port that the host is listening on (valid for LAN/GameServer) */
	TSharedPtr<class FInternetAddr> HostAddr;
	/** The Steam P2P address that the host is listening on (valid for GameServer/Lobby) */
	TSharedPtr<class FInternetAddr> SteamP2PAddr;
	/** Steam Lobby Id or Gameserver Id if applicable */
	FUniqueNetIdEOS SessionId;
	/** How this session should be connected to */
	FEOSConnectionMethod ConnectionMethod;

public:

	virtual ~FOnlineSessionInfoEOS() {}

	/**
	 *	Comparison operator
	 */
	bool operator==( const FOnlineSessionInfoEOS& Other ) const
	{
		return false;
	}

	virtual const uint8* GetBytes() const override
	{
		return nullptr;
	}

	virtual int32 GetSize() const override
	{
		return sizeof( uint64 ) +
			sizeof( EEOSSession::Type ) +
			sizeof( TSharedPtr<class FInternetAddr> ) +
			sizeof( TSharedPtr<class FInternetAddr> ) +
			sizeof( FUniqueNetIdEOS ) +
			sizeof( FEOSConnectionMethod );
	}

	virtual bool IsValid() const override
	{
		switch( SessionType )
		{
		case EEOSSession::LobbySession:
			//return SteamP2PAddr.IsValid() && SteamP2PAddr->IsValid() && SessionId.IsValid();
		case EEOSSession::AdvertisedSessionHost:
		case EEOSSession::AdvertisedSessionClient:
			//return ( ( SteamP2PAddr.IsValid() && SteamP2PAddr->IsValid() ) || ( HostAddr.IsValid() && HostAddr->IsValid() ) ) && SessionId.IsValid();
		case EEOSSession::LANSession:
		default:
			// LAN case
			return HostAddr.IsValid() && HostAddr->IsValid();
		}
	}

	virtual FString ToString() const override
	{
		return SessionId.ToString();
	}

	virtual FString ToDebugString() const override
	{
		return FString::Printf( TEXT( "HostIP: %s EOSP2P: %s Type: %s SessionId: %s" ),
								HostAddr.IsValid() ? *HostAddr->ToString( true ) : TEXT( "INVALID" ),
								SteamP2PAddr.IsValid() ? *SteamP2PAddr->ToString( true ) : TEXT( "INVALID" ),
								EEOSSession::ToString( SessionType ), *SessionId.ToDebugString() );
	}

	virtual const FUniqueNetId& GetSessionId() const override
	{
		return SessionId;
	}
};
