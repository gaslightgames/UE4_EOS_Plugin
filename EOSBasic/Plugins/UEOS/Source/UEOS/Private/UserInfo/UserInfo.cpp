// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UEOS Includes
#include "UserInfo/UserInfo.h"

#include "UEOSModule.h"
#include "UEOSManager.h"

UEOSUserInfo::UEOSUserInfo()
{

}

void UEOSUserInfo::QueryUserInfoByAccountId( const FEpicAccountId& EpicAccountId )
{
	EOS_HUserInfo UserInfoHandle = EOS_Platform_GetUserInfoInterface(UEOSManager::GetPlatformHandle());
	EOS_UserInfo_QueryUserInfoOptions Options = {};
	Options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId().EpicAccountId;
	Options.TargetUserId = EpicAccountId.EpicAccountId;

	EOS_UserInfo_QueryUserInfo( UserInfoHandle, &Options, nullptr, QueryUserInfoCallback );
}

FString UEOSUserInfo::GetDisplayName( const FEpicAccountId& EpicAccountId )
{
	EOS_HUserInfo UserInfoHandle = EOS_Platform_GetUserInfoInterface(UEOSManager::GetPlatformHandle());
	EOS_UserInfo_CopyUserInfoOptions Options;
	Options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetEpicAccountId();
	Options.TargetUserId = EpicAccountId;

	EOS_UserInfo* UserInfo = nullptr;

	EOS_EResult ResultCode = EOS_UserInfo_CopyUserInfo( UserInfoHandle, &Options, &UserInfo );

	if( ResultCode != EOS_EResult::EOS_Success )
	{
		UE_LOG( UEOSLog, Warning, TEXT( "[EOS SDK | Plugin] Error when getting display name: %s" ), *UEOSCommon::EOSResultToString( ResultCode ) );
	}

	if( UserInfo == nullptr )
	{
		return FString();
	}

	FString Result = FString( UTF8_TO_TCHAR( UserInfo->DisplayName ) );

	EOS_UserInfo_Release( UserInfo );

	return Result;
}

void UEOSUserInfo::QueryUserInfoCallback( const EOS_UserInfo_QueryUserInfoCallbackInfo* Data )
{
	check( Data != nullptr );

	UEOSUserInfo* EOSUserInfo = UEOSManager::GetUserInfo();

	if( Data->ResultCode == EOS_EResult::EOS_Success )
	{
		if( EOSUserInfo != nullptr )
		{
			FEpicAccountId UpdatedEpicAccountId = FEpicAccountId(Data->TargetUserId);
			UpdatedEpicAccountId.DisplayName = GetDisplayName(Data->TargetUserId);
			EOSUserInfo->OnUserInfoRetreived.Broadcast(UpdatedEpicAccountId);
		}
	}
	else
	{
		UE_LOG( UEOSLog, Warning, TEXT( "[EOS SDK | Plugin] Error when querying user info: %s" ), *UEOSCommon::EOSResultToString( Data->ResultCode ) );
		if( EOSUserInfo != nullptr )
		{
			FEpicAccountId UpdatedEpicAccountId = FEpicAccountId(Data->TargetUserId);
			UpdatedEpicAccountId.DisplayName = GetDisplayName(Data->TargetUserId);
			EOSUserInfo->OnUserInfoError.Broadcast(UpdatedEpicAccountId);
		}
	}
}
