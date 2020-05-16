// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UEOS Includes
#include "UserInfo/UserInfo.h"

#include "UEOSModule.h"
#include "UEOSManager.h"

UEOSUserInfo::UEOSUserInfo()
	: UserInfoHandle( NULL )
{

}

void UEOSUserInfo::Init()
{
	UserInfoHandle = EOS_Platform_GetUserInfoInterface( UEOSManager::GetPlatformHandle() );
}

void UEOSUserInfo::QueryNameByAccountId( const FAccountId& AccountId )
{
	EOS_UserInfo_QueryUserInfoOptions Options;
	Options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetAccountId();
	Options.TargetUserId = AccountId;

	EOS_UserInfo_QueryUserInfo( UserInfoHandle, &Options, nullptr, QueryUserInfoCallback );
}

FString UEOSUserInfo::GetDisplayName( const FAccountId& AccountId )
{
	EOS_UserInfo_CopyUserInfoOptions Options;
	Options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
	Options.LocalUserId = UEOSManager::GetEOSManager()->GetAuthentication()->GetAccountId();
	Options.TargetUserId = AccountId;

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
			EOSUserInfo->OnUserInfoRetreived.Broadcast( FAccountId( Data->TargetUserId ) );
		}
	}
	else
	{
		UE_LOG( UEOSLog, Warning, TEXT( "[EOS SDK | Plugin] Error when querying user info: %s" ), *UEOSCommon::EOSResultToString( Data->ResultCode ) );
		if( EOSUserInfo != nullptr )
		{
			EOSUserInfo->OnUserInfoError.Broadcast( FAccountId( Data->TargetUserId ) );
		}
	}
}
