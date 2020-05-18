// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UEOS Includes
#include "Friends/Friends.h"

#include "UEOSModule.h"
#include "UEOSManager.h"
#include "UserInfo/UserInfo.h"

UEOSFriends::UEOSFriends()
{

}

void UEOSFriends::QueryFriends()
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
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

int32 UEOSFriends::GetFriendsCount()
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_GetFriendsCountOptions Options;
	Options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();

	int32_t FriendCount = EOS_Friends_GetFriendsCount(FriendsHandle, &Options);
	
	return FriendCount;
}

void UEOSFriends::RequestFriendId( int32 Index )
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_GetFriendAtIndexOptions Options;
	Options.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	Options.Index = Index;
	EOS_EpicAccountId EpicAccountId = EOS_Friends_GetFriendAtIndex( FriendsHandle, &Options );

	UEOSUserInfo* UserInfo = UEOSManager::GetEOSManager()->GetUserInfo();
	UserInfo->QueryUserInfoByAccountId(EpicAccountId);
}

/* =================== FRIEND MESSAGING FUNCTIONS ============================= */
void UEOSFriends::SendInvite(const FEpicAccountId& FriendInfo)
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_SendInviteOptions Options;
	Options.ApiVersion = EOS_FRIENDS_SENDINVITE_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();

	FEpicAccountId BlankId;
	FEpicAccountId FriendId = BlankId.FromString(FriendInfo.ToString());
	Options.TargetUserId = FriendId;
	EOS_Friends_SendInvite(FriendsHandle, &Options, NULL, SendInviteCallback);
}

void UEOSFriends::SendInviteCallback(const EOS_Friends_SendInviteCallbackInfo* Data)
{
	check(Data != nullptr);

	UEOSFriends* EOSFriends = UEOSManager::GetFriends();
	if (EOSFriends != nullptr)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			EOSFriends->OnFriendInviteSent.Broadcast();
		}
		else
		{
			FString DisplayName = UEOSManager::GetEOSManager()->GetUserInfo()->GetDisplayName(Data->TargetUserId);
			UE_LOG(UEOSLog, Warning, TEXT("[EOS SDK | Plugin] Error %s when sending invite to %s"), *UEOSCommon::EOSResultToString(Data->ResultCode), *DisplayName);
			EOSFriends->OnFriendSendInviteError.Broadcast();
		}
	}
}

void UEOSFriends::AcceptInvite(const FEpicAccountId& FriendInfo)
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_AcceptInviteOptions Options;
	Options.ApiVersion = EOS_FRIENDS_ACCEPTINVITE_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	
	FEpicAccountId BlankId;
	FEpicAccountId FriendId = BlankId.FromString(FriendInfo.ToString());
	Options.TargetUserId = FriendId;
	EOS_Friends_AcceptInvite(FriendsHandle, &Options, NULL, AcceptInviteCallback);
}

void UEOSFriends::AcceptInviteCallback(const EOS_Friends_AcceptInviteCallbackInfo* Data)
{
	check(Data != nullptr);

	UEOSFriends* EOSFriends = UEOSManager::GetFriends();
	if (EOSFriends != nullptr)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			EOSFriends->OnFriendInviteAccepted.Broadcast();
		}
		else
		{
			FString DisplayName = UEOSManager::GetEOSManager()->GetUserInfo()->GetDisplayName(Data->TargetUserId);
			UE_LOG(UEOSLog, Warning, TEXT("[EOS SDK | Plugin] Error %s when accepting invite from friend: %s"), *UEOSCommon::EOSResultToString(Data->ResultCode), *DisplayName);
			EOSFriends->OnFriendAcceptInviteError.Broadcast();
		}
	}
}

void UEOSFriends::RejectInvite(const FEpicAccountId& FriendInfo)
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_RejectInviteOptions Options;
	Options.ApiVersion = EOS_FRIENDS_REJECTINVITE_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	FEpicAccountId BlankId;
	FEpicAccountId FriendId = BlankId.FromString(FriendInfo.ToString());
	Options.TargetUserId = FriendId;
	EOS_Friends_RejectInvite(FriendsHandle, &Options, NULL, RejectInviteCallback);
}

void UEOSFriends::RejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data)
{
	check(Data != nullptr);

	UEOSFriends* EOSFriends = UEOSManager::GetFriends();
	if (EOSFriends != nullptr)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			EOSFriends->OnFriendInviteRejected.Broadcast();
		}
		else
		{
			FString DisplayName = UEOSManager::GetEOSManager()->GetUserInfo()->GetDisplayName(Data->TargetUserId);
			UE_LOG(UEOSLog, Warning, TEXT("[EOS SDK | Plugin] Error %s when rejecting invite from friend: %s"), *UEOSCommon::EOSResultToString(Data->ResultCode), *DisplayName);
			EOSFriends->OnFriendRejectedError.Broadcast();
		}
	}
}
/* =================== END FRIEND MESSAGING FUNCTIONS ============================= */

EFriendStatus UEOSFriends::GetStatus( const FEpicAccountId& FriendInfo)
{

	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_GetStatusOptions StatusOptions;
	StatusOptions.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
	StatusOptions.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	FEpicAccountId BlankId;
	FEpicAccountId FriendId = BlankId.FromString(FriendInfo.ToString());
	StatusOptions.TargetUserId = FriendId;
	EOS_EFriendsStatus Status = EOS_Friends_GetStatus( FriendsHandle, &StatusOptions );
	return (EFriendStatus)Status;
}



void UEOSFriends::AddNotifyFriendsHandle()
{
	EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(UEOSManager::GetPlatformHandle());
	EOS_Friends_AddNotifyFriendsUpdateOptions Options;
	Options.ApiVersion = EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST;
	EOS_Friends_AddNotifyFriendsUpdate(FriendsHandle, &Options, NULL, OnFriendsUpdateCallback);
}

void UEOSFriends::OnFriendsUpdateCallback(const EOS_Friends_OnFriendsUpdateInfo* Data)
{
	check(Data != nullptr);

	//TODO - This will have to be used in combination with AddFriend and RemoveFriend
}