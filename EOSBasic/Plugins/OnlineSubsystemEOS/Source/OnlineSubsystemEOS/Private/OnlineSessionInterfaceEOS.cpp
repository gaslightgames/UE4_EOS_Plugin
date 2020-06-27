// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "OnlineSessionInterfaceEOS.h"


TSharedPtr<const FUniqueNetId> FOnlineSessionEOS::CreateSessionIdFromString( const FString& SessionIdStr )
{
	if( !SessionIdStr.IsEmpty() )
	{
		return MakeShared<FUniqueNetIdEOS>( SessionIdStr );
	}

	return nullptr;
}

FNamedOnlineSession* FOnlineSessionEOS::AddNamedSession( FName SessionName, const FOnlineSessionSettings& SessionSettings )
{
	FScopeLock ScopeLock( &SessionLock );
	return new ( Sessions ) FNamedOnlineSession( SessionName, SessionSettings );
}

FNamedOnlineSession* FOnlineSessionEOS::AddNamedSession( FName SessionName, const FOnlineSession& Session )
{
	FScopeLock ScopeLock( &SessionLock );
	return new ( Sessions ) FNamedOnlineSession( SessionName, Session );
}

FNamedOnlineSession* FOnlineSessionEOS::GetNamedSession( FName SessionName )
{
	FScopeLock ScopeLock( &SessionLock );
	for( int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++ )
	{
		if( Sessions[SearchIndex].SessionName == SessionName )
		{
			return &Sessions[SearchIndex];
		}
	}

	return nullptr;
}

FNamedOnlineSession* FOnlineSessionEOS::GetNamedSessionFromLobbyId( FUniqueNetIdEOS& LobbyId )
{
	FScopeLock ScopeLock( &SessionLock );
	for( int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++ )
	{
		FNamedOnlineSession& Session = Sessions[SearchIndex];
		if( Session.SessionInfo.IsValid() )
		{
			FOnlineSessionInfoEOS* SessionInfo = (FOnlineSessionInfoEOS*)Session.SessionInfo.Get();
			if( SessionInfo->SessionType == EEOSSession::LobbySession && SessionInfo->SessionId == LobbyId )
			{
				return &Sessions[SearchIndex];
			}
		}
	}
	return nullptr;
}

void FOnlineSessionEOS::RemoveNamedSession( FName SessionName )
{
	FScopeLock ScopeLock( &SessionLock );
	for( int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++ )
	{
		if( Sessions[SearchIndex].SessionName == SessionName )
		{
			Sessions.RemoveAtSwap( SearchIndex );
			return;
		}
	}
}

bool FOnlineSessionEOS::HasPresenceSession()
{
	FScopeLock ScopeLock( &SessionLock );
	for( int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++ )
	{
		if( Sessions[SearchIndex].SessionSettings.bUsesPresence )
		{
			return true;
		}
	}

	return false;
}

EOnlineSessionState::Type FOnlineSessionEOS::GetSessionState( FName SessionName ) const
{
	FScopeLock ScopeLock( &SessionLock );
	for( int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++ )
	{
		if( Sessions[SearchIndex].SessionName == SessionName )
		{
			return Sessions[SearchIndex].SessionState;
		}
	}

	return EOnlineSessionState::NoSession;
}

bool FOnlineSessionEOS::CreateSession( int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings )
{
	return false;
}

bool FOnlineSessionEOS::CreateSession( const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings )
{
	return false;
}

bool FOnlineSessionEOS::StartSession( FName SessionName )
{
	return false;
}

bool FOnlineSessionEOS::UpdateSession( FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData )
{
	return false;
}

bool FOnlineSessionEOS::EndSession( FName SessionName )
{
	return false;
}

bool FOnlineSessionEOS::DestroySession( FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate )
{
	return false;
}

bool FOnlineSessionEOS::IsPlayerInSession( FName SessionName, const FUniqueNetId& UniqueId )
{
	return false;
}

bool FOnlineSessionEOS::StartMatchmaking( const TArray< TSharedRef<const FUniqueNetId> >& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings )
{
	return false;
}

bool FOnlineSessionEOS::CancelMatchmaking( int32 SearchingPlayerNum, FName SessionName )
{
	return false;
}

bool FOnlineSessionEOS::CancelMatchmaking( const FUniqueNetId& SearchingPlayerId, FName SessionName )
{
	return false;
}

bool FOnlineSessionEOS::FindSessions( int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings )
{
	return false;
}

bool FOnlineSessionEOS::FindSessions( const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings )
{
	return false;
}

bool FOnlineSessionEOS::FindSessionById( const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate )
{
	return false;
}

bool FOnlineSessionEOS::CancelFindSessions()
{
	return false;
}

bool FOnlineSessionEOS::PingSearchResults( const FOnlineSessionSearchResult& SearchResult )
{
	return false;
}

bool FOnlineSessionEOS::JoinSession( int32 PlayerNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession )
{
	return false;
}

bool FOnlineSessionEOS::JoinSession( const FUniqueNetId& PlayerId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession )
{
	return false;
}

bool FOnlineSessionEOS::FindFriendSession( int32 LocalUserNum, const FUniqueNetId& Friend )
{
	return false;
}

bool FOnlineSessionEOS::FindFriendSession( const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend )
{
	return false;
}

bool FOnlineSessionEOS::FindFriendSession( const FUniqueNetId& LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& FriendList )
{
	return false;
}

bool FOnlineSessionEOS::SendSessionInviteToFriend( int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend )
{
	return false;
}

bool FOnlineSessionEOS::SendSessionInviteToFriend( const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend )
{
	return false;
}

bool FOnlineSessionEOS::SendSessionInviteToFriends( int32 LocalUserNum, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends )
{
	return false;
}

bool FOnlineSessionEOS::SendSessionInviteToFriends( const FUniqueNetId& LocalUserId, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends )
{
	return false;
}

bool FOnlineSessionEOS::GetResolvedConnectString( FName SessionName, FString& ConnectInfo, FName PortType )
{
	return false;
}

bool FOnlineSessionEOS::GetResolvedConnectString( const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo )
{
	return false;
}

FOnlineSessionSettings* FOnlineSessionEOS::GetSessionSettings( FName SessionName )
{
	return nullptr;
}

bool FOnlineSessionEOS::RegisterPlayer( FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited )
{
	return false;
}

bool FOnlineSessionEOS::RegisterPlayers( FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Players, bool bWasInvited )
{
	return false;
}

bool FOnlineSessionEOS::UnregisterPlayer( FName SessionName, const FUniqueNetId& PlayerId )
{
	return false;
}

bool FOnlineSessionEOS::UnregisterPlayers( FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Players )
{
	return false;
}

void FOnlineSessionEOS::RegisterLocalPlayer( const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate )
{

}

void FOnlineSessionEOS::UnregisterLocalPlayer( const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate )
{

}

int32 FOnlineSessionEOS::GetNumSessions()
{
	return -1;
}

void FOnlineSessionEOS::DumpSessionState()
{

}

/** Implementation of the ConnectionMethod converters */
FString LexToString( const FEOSConnectionMethod Method )
{
	switch( Method )
	{
	default:
	case FEOSConnectionMethod::None:
		return TEXT( "None" );
	case FEOSConnectionMethod::Direct:
		return TEXT( "Direct" );
	case FEOSConnectionMethod::P2P:
		return TEXT( "P2P" );
	case FEOSConnectionMethod::PartnerHosted:
		return TEXT( "PartnerHosted" );
	}
}

FEOSConnectionMethod ToConnectionMethod( const FString& InString )
{
	if( InString == TEXT( "Direct" ) )
	{
		return FEOSConnectionMethod::Direct;
	}
	else if( InString == TEXT( "P2P" ) )
	{
		return FEOSConnectionMethod::P2P;
	}
	else if( InString == TEXT( "PartnerHosted" ) )
	{
		return FEOSConnectionMethod::PartnerHosted;
	}
	else
	{
		return FEOSConnectionMethod::None;
	}
}
