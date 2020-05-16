// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UEOS Includes
#include "Friends/Friends.h"

#include "UEOSModule.h"
#include "UEOSManager.h"

UEOSFriends::UEOSFriends()
	: FriendsHandle( NULL )
{

}

void UEOSFriends::Init()
{
	FriendsHandle = EOS_Platform_GetFriendsInterface( UEOSManager::GetPlatformHandle() );
}

void UEOSFriends::RefreshFriends()
{
	EOS_Friends_QueryFriendsOptions Options;
	Options.ApiVersion = EOS_FRIENDS_QUERYFRIENDS_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	EOS_Friends_QueryFriends( FriendsHandle, &Options, nullptr, QueryFriendsCallback );
}

void UEOSFriends::QueryFriendsCallback( const EOS_Friends_QueryFriendsCallbackInfo* Data )
{
	check( Data != nullptr );

	UEOSFriends* EOSFriends = UEOSManager::GetFriends();
	if( EOSFriends != nullptr )
	{
		if( Data->ResultCode == EOS_EResult::EOS_Success )
		{
			EOSFriends->OnFriendsRefreshed.Broadcast();
		}
		else
		{
			UE_LOG( UEOSLog, Warning, TEXT( "[EOS SDK | Plugin] Error when querying friends: %s" ), *UEOSCommon::EOSResultToString( Data->ResultCode ) );
			EOSFriends->OnRefreshFriendsError.Broadcast();
		}
	}
}

int UEOSFriends::GetFriendsCount()
{
	EOS_Friends_GetFriendsCountOptions Options;
	Options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	return EOS_Friends_GetFriendsCount( FriendsHandle, &Options );
}

FEpicAccountId UEOSFriends::GetEpicAccountId( int Index )
{
	EOS_Friends_GetFriendAtIndexOptions Options;
	Options.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	Options.Index = Index;
	EOS_EpicAccountId EpicAccountId = EOS_Friends_GetFriendAtIndex( FriendsHandle, &Options );
	return EpicAccountId;
}

EFriendStatus UEOSFriends::GetStatus( FEpicAccountId EpicAccountId )
{
	EOS_Friends_GetStatusOptions StatusOptions;
	StatusOptions.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
	StatusOptions.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	StatusOptions.TargetUserId = EpicAccountId;
	EOS_EFriendsStatus Status = EOS_Friends_GetStatus( FriendsHandle, &StatusOptions );
	return (EFriendStatus)Status;
}

