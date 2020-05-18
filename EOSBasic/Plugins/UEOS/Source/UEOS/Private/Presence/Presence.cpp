// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UEOS Includes
#include "Presence/Presence.h"
#include "UEOSManager.h"
#include "UserInfo/UserInfo.h"

#include "UEOSModule.h"

//TODO - Class is in the works - will be used with friend interface - Mikhail 

UEOSPresence::UEOSPresence()
{

}

EPresenceStatus UEOSPresence::GetPresenceStatus(const FEpicAccountId& InFriendInfo)
{
	EOS_HPresence PresenceHandle = EOS_Platform_GetPresenceInterface(UEOSManager::GetPlatformHandle());
	
	EOS_Presence_CopyPresenceOptions CopyOptions;
	CopyOptions.ApiVersion = EOS_PRESENCE_COPYPRESENCE_API_LATEST;
	CopyOptions.LocalUserId = UEOSManager::GetAuthentication()->GetEpicAccountId();

	FEpicAccountId AccountId = FEpicAccountId();
	CopyOptions.TargetUserId = AccountId.FromString(InFriendInfo.ToString());

	EOS_Presence_Info* PresenceInfo = nullptr;
	EOS_EResult ResultCode = EOS_Presence_CopyPresence(PresenceHandle, &CopyOptions, &PresenceInfo);

	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG(UEOSLog, Warning, TEXT("[EOS SDK | Plugin] Error when getting presence status: %s"), *UEOSCommon::EOSResultToString(ResultCode));
	}
	
	EPresenceStatus PresenceStatus = (EPresenceStatus)PresenceInfo->Status;

	EOS_Presence_Info_Release(PresenceInfo);

	return PresenceStatus;
}

void UEOSPresence::AddFriendPresence(const FEpicAccountId& InFriendInfo)
{
	EOS_HPresence PresenceHandle = EOS_Platform_GetPresenceInterface(UEOSManager::GetPlatformHandle());
	FEpicAccountId AccountId = FEpicAccountId();
	EOS_Presence_QueryPresenceOptions Options;
	Options.ApiVersion = EOS_PRESENCE_QUERYPRESENCE_API_LATEST;
	Options.LocalUserId = UEOSManager::GetAuthentication()->GetEpicAccountId();
	Options.TargetUserId = AccountId.FromString(InFriendInfo.ToString());
	EOS_Presence_QueryPresence(PresenceHandle, &Options, NULL, QueryPresenceCompleteCallback);
}

void UEOSPresence::QueryPresenceCompleteCallback(const EOS_Presence_QueryPresenceCallbackInfo* Data)
{
	check(Data != nullptr);

	UE_LOG(UEOSLog, Log, TEXT("On query presence result code: %s"), *UEOSCommon::EOSResultToString(Data->ResultCode));

	UEOSUserInfo* UserInfo = UEOSManager::GetEOSManager()->GetUserInfo();
	UserInfo->QueryUserInfoByAccountId(Data->TargetUserId);

	SetPresence(Data->TargetUserId);

}


void UEOSPresence::SetPresence(FEpicAccountId TargetUserId)
{
	EOS_HPresence PresenceHandle = EOS_Platform_GetPresenceInterface(UEOSManager::GetPlatformHandle());
	EOS_Presence_SetPresenceOptions Options;
	Options.ApiVersion = EOS_PRESENCE_SETPRESENCE_API_LATEST;
	Options.LocalUserId = UEOSManager::GetAuthentication()->GetEpicAccountId();

	EOS_HPresenceModification PresenceModificationHandle;
	EOS_Presence_CreatePresenceModificationOptions PresenceModificationOptions;
	PresenceModificationOptions.LocalUserId = UEOSManager::GetAuthentication()->GetEpicAccountId();
	PresenceModificationOptions.ApiVersion = EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST;
	EOS_EResult EResult = EOS_Presence_CreatePresenceModification(PresenceHandle, &PresenceModificationOptions, &PresenceModificationHandle);

	if (EResult == EOS_EResult::EOS_Success)
	{
		Options.PresenceModificationHandle = PresenceModificationHandle;

		EOS_Presence_SetPresence(PresenceHandle, &Options, TargetUserId, SetPresenceCallback);

		EOS_PresenceModification_Release(PresenceModificationHandle);

	} else
	{
		UE_LOG(UEOSLog, Log, TEXT("%s: threw error of: %s"), __FUNCTIONW__, *UEOSCommon::EOSResultToString(EResult));
	}

}

void UEOSPresence::SetPresenceCallback(const EOS_Presence_SetPresenceCallbackInfo* Data)
{
	check(Data != nullptr);

	UE_LOG(UEOSLog, Log, TEXT("On set presence result code: %s"), *UEOSCommon::EOSResultToString(Data->ResultCode));

	
}


void UEOSPresence::RequestFriendPresences()
{
	EOS_HPresence PresenceHandle = EOS_Platform_GetPresenceInterface(UEOSManager::GetPlatformHandle());
	EOS_Presence_AddNotifyOnPresenceChangedOptions Options;
	Options.ApiVersion = EOS_PRESENCE_ADDNOTIFYONPRESENCECHANGED_API_LATEST;
	EOS_Presence_AddNotifyOnPresenceChanged(PresenceHandle, &Options, NULL, OnPresenceChangedCallback);
}

void UEOSPresence::OnPresenceChangedCallback(const EOS_Presence_PresenceChangedCallbackInfo* Data)
{
	check(Data != nullptr);

	UEOSUserInfo* EOSUserInfo = UEOSManager::GetUserInfo();

	/*if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		if (EOSUserInfo != nullptr)
		{
			FBPCrossPlayFriendInfo FriendInfo;
			FAccountId TargetID = FAccountId(Data->TargetUserId);
			FriendInfo.ID = TargetID.ToString();

			FriendInfo.DisplayName = GetDisplayName(TargetID);
			EOSUserInfo->OnUserInfoRetreived.Broadcast(FriendInfo);
		}
	}
	else
	{
		UE_LOG(UEOSLog, Warning, TEXT("[EOS SDK | Plugin] Error when querying user info: %s"), *UEOSCommon::EOSResultToString(Data->ResultCode));
		if (EOSUserInfo != nullptr)
		{
			EOSUserInfo->OnReachedLastUserInfo.Broadcast(FBPCrossPlayFriendInfo());
		}
	}*/
	
	//UEOSUserInfo* UserInfo = UEOSManager::GetEOSManager()->GetUserInfo();
	//UserInfo->QueryNameByAccountId(Data->PresenceUserId);
}


