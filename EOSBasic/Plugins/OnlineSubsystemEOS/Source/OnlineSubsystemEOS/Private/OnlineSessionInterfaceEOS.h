// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UE4 Includes
#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "Misc/ScopeLock.h"
#include "OnlineKeyValuePair.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

// EOS Includes
#include "OnlineSubsystemEOSTypes.h"


// Forward Declarations
class FOnlineSubsystemEOS;
class FLANSession;
class FNamedOnlineSession;



/**
 * Interface definition for the online services session services
 * Session services are defined as anything related managing a session
 * and its state within a platform service
 */
class FOnlineSessionEOS : public IOnlineSession
{

public:

PACKAGE_SCOPE :

	FOnlineSessionEOS( FOnlineSubsystemEOS* InSubsystem )
		: EOSSubsystem( InSubsystem )
		, LANSession( nullptr )
	{}

	virtual ~FOnlineSessionEOS() {}

	// IOnlineSession

	virtual TSharedPtr<const FUniqueNetId>	CreateSessionIdFromString( const FString& SessionIdStr ) override;

	/**
	 * Adds a new named session to the list (new session)
	 *
	 * @param SessionName the name to search for
	 * @param GameSettings the game settings to add
	 *
	 * @return a pointer to the struct that was added
	 */
	FNamedOnlineSession*					AddNamedSession( FName SessionName, const FOnlineSessionSettings& SessionSettings ) override;
	
	/**
	 * Adds a new named session to the list (from existing session data)
	 *
	 * @param SessionName the name to search for
	 * @param GameSettings the game settings to add
	 *
	 * @return a pointer to the struct that was added
	 */
	FNamedOnlineSession*					AddNamedSession( FName SessionName, const FOnlineSession& Session ) override;
	
	FNamedOnlineSession*					GetNamedSession( FName SessionName ) override;

	/**
	 * Searches the named session array for the specified session
	 *
	 * @param LobbyId the lobby id to search for
	 *
	 * @return pointer to the struct if found, NULL otherwise
	 */
	inline FNamedOnlineSession*				GetNamedSessionFromLobbyId( FUniqueNetIdEOS& LobbyId );
	
	virtual void							RemoveNamedSession( FName SessionName ) override;
	virtual bool							HasPresenceSession() override;
	virtual EOnlineSessionState::Type		GetSessionState( FName SessionName ) const override;

	virtual bool							CreateSession( int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings ) override;
	virtual bool							CreateSession( const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings ) override;
	virtual bool							StartSession( FName SessionName ) override;
	virtual bool							UpdateSession( FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData = true ) override;
	virtual bool							EndSession( FName SessionName ) override;
	virtual bool							DestroySession( FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate = FOnDestroySessionCompleteDelegate() ) override;
	virtual bool							IsPlayerInSession( FName SessionName, const FUniqueNetId& UniqueId ) override;
	virtual bool							StartMatchmaking( const TArray< TSharedRef<const FUniqueNetId> >& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings ) override;
	virtual bool							CancelMatchmaking( int32 SearchingPlayerNum, FName SessionName ) override;
	virtual bool							CancelMatchmaking( const FUniqueNetId& SearchingPlayerId, FName SessionName ) override;
	virtual bool							FindSessions( int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings ) override;
	virtual bool							FindSessions( const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings ) override;
	virtual bool							FindSessionById( const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate ) override;
	virtual bool							CancelFindSessions() override;
	virtual bool							PingSearchResults( const FOnlineSessionSearchResult& SearchResult ) override;
	virtual bool							JoinSession( int32 PlayerNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession ) override;
	virtual bool							JoinSession( const FUniqueNetId& PlayerId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession ) override;
	virtual bool							FindFriendSession( int32 LocalUserNum, const FUniqueNetId& Friend ) override;
	virtual bool							FindFriendSession( const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend ) override;
	virtual bool							FindFriendSession( const FUniqueNetId& LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& FriendList ) override;
	virtual bool							SendSessionInviteToFriend( int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend ) override;
	virtual bool							SendSessionInviteToFriend( const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend ) override;
	virtual bool							SendSessionInviteToFriends( int32 LocalUserNum, FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Friends ) override;
	virtual bool							SendSessionInviteToFriends( const FUniqueNetId& LocalUserId, FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Friends ) override;
	virtual bool							GetResolvedConnectString( FName SessionName, FString& ConnectInfo, FName PortType ) override;
	virtual bool							GetResolvedConnectString( const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo ) override;
	virtual FOnlineSessionSettings*			GetSessionSettings( FName SessionName ) override;
	virtual bool							RegisterPlayer( FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited ) override;
	virtual bool							RegisterPlayers( FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Players, bool bWasInvited = false ) override;
	virtual bool							UnregisterPlayer( FName SessionName, const FUniqueNetId& PlayerId ) override;
	virtual bool							UnregisterPlayers( FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Players ) override;
	virtual void							RegisterLocalPlayer( const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate ) override;
	virtual void							UnregisterLocalPlayer( const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate ) override;
	virtual int32							GetNumSessions() override;
	virtual void							DumpSessionState() override;

protected:

	/** Critical sections for thread safe operation of session lists */
	mutable FCriticalSection				SessionLock;

	/** Current session settings */
	TArray<FNamedOnlineSession>				Sessions;

private:

	/** Reference to the main EOS subsystem */
	FOnlineSubsystemEOS*					EOSSubsystem;

	/** Instance of a LAN session for hosting/client searches */
	FLANSession*							LANSession;

	/** Hidden on purpose */
	FOnlineSessionEOS()
		: EOSSubsystem( nullptr )
		, LANSession( nullptr )
	{}

};

typedef TSharedPtr<FOnlineSessionEOS, ESPMode::ThreadSafe> FOnlineSessionEOSPtr;
