// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

#include "UObject/Object.h"

#include "Authentication/Authentication.h"
#include "eos_sdk.h"
#include "eos_presence.h"
#include "eos_presence_types.h"

#include "Friends/Friends.h"

#include "Presence.generated.h"

/** The current status of a friend's online presence.  EPresenceStatus as a UENUM. */
UENUM(BlueprintType)
enum class EPresenceStatus : uint8
{
	Offline = 0			UMETA(DisplayName = "Offline"),		/** Offline Presence */
	Online				UMETA(DisplayName = "Online"),		/** Online Presence */
	Away				UMETA(DisplayName = "Away"),		/** Away Presence */
	ExtendedAway		UMETA(DisplayName = "ExtendedAway"),/** Extended Away - Longer than 1 hour away **/
	DoNotDisturb		UMETA(DisplayName = "DND")			/** Do not Disturb **/
};

//TODO - Format
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	Epic = 0,
	Steam = 1,
	Oculus = 2,
	Playstation = 3,
	OTHER = 4
};

USTRUCT(BlueprintType)
struct FBPCrossPlayInfo
{

	GENERATED_USTRUCT_BODY()

	FBPCrossPlayInfo() = default;

	FBPCrossPlayInfo(const FBPCrossPlayInfo&) = default;

	FBPCrossPlayInfo& operator=(const FBPCrossPlayInfo&) = default;
	
	bool operator==(const FBPCrossPlayInfo& Other) const
	{
		return IdAsString == Other.IdAsString;
	}

	bool operator!=(const FBPCrossPlayInfo& Other) const
	{
		return !(this->operator==(Other));
	}

	bool operator<(const FBPCrossPlayInfo& Other) const
	{
		return IdAsString < Other.IdAsString;
	}

	UPROPERTY(BlueprintReadWrite, Category = "UEOS|Friends|CrossPlay")
		FString IdAsString;

	UPROPERTY(BlueprintReadWrite, Category = "UEOS|Friends|CrossPlay")
		FString DisplayName;

	UPROPERTY(BlueprintReadWrite, Category = "UEOS|Friends|CrossPlay")
		EPresenceStatus Presence;

	UPROPERTY(BlueprintReadWrite, Category = "UEOS|Friends|CrossPlay")
		EPlatformType PlatformType = EPlatformType::Epic;

	/**
	* Returns an Account ID from a String interpretation of one.
	*
	* @param AccountId the FString representation of an Account ID.
	* @return FEpicAccountId An Account ID from the string, if valid.
	*/
	/*static FEpicAccountId		FromString(const FString& AccountId)
	{
		EOS_EpicAccountId Account = EOS_EpicAccountId_FromString(TCHAR_TO_ANSI(*AccountId));
		return FEpicAccountId(Account);
	}*/
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnFriendPresenceQueryComplete, const FBPCrossPlayInfo&, CompleteFriendInfo);

UCLASS()
class UEOS_API UEOSPresence : public UObject
{
	GENERATED_BODY()

public:

	UEOSPresence();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Presence" )
		void QueryFriendPresence( const FBPCrossPlayInfo& InFriendInfo );

	/**
	 * Begins an async process that requests the presence statuses of the friends.
	 */
	UFUNCTION(BlueprintCallable, Category = "UEOS|Presence")
		void SubscribeToFriendPresenceUpdates();

	UPROPERTY( BlueprintAssignable , Category = "UEOS|Presence")
		FOnFriendPresenceQueryComplete OnFriendPresenceQueryComplete;

protected:

	static void UpdatePresenceStatus(FBPCrossPlayInfo& InFriendInfo, FEpicAccountId TargetId);

	static void SetPresenceCallback(const EOS_Presence_SetPresenceCallbackInfo* Data);

	static void QueryPresenceCompleteCallback(const EOS_Presence_QueryPresenceCallbackInfo* Data);

	static void OnPresenceChangedCallback(const EOS_Presence_PresenceChangedCallbackInfo* Data);

	static void SetPresence(FEpicAccountId TargetUserId);

};

