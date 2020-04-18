// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

#include "UObject/Object.h"

#include "Authentication/Authentication.h"
#include "eos_sdk.h"
#include "eos_friends.h"

#include "Friends.generated.h"

/** The current status of a friendship.  EOS_EFriendsStatus as a UENUM. */
UENUM(BlueprintType)
enum class EFriendStatus : uint8
{
	NotFriends = 0,
	InviteSent = 1,
	InviteReceived = 2,
	Friends = 3,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnFriendsRefreshed );

UCLASS()
class UEOS_API UEOSFriends : public UObject
{
	GENERATED_BODY()

public:

	UEOSFriends();

	void Init();

	/**
	 * Begins an async process that requests the friends count, account ids, and statuses of the local player's friends.
	 * Broadcasts OnFriendsRefreshed when completed.
	 */
	void						RefreshFriends();

	int							GetFriendsCount();
	FAccountId					GetAccountId( int Index );
	EFriendStatus				GetStatus( FAccountId AccountId );

	/**
	 * Fires when a call to RefreshFriends succeeds
	 */
	UPROPERTY( BlueprintAssignable, Category = "UEOS|Friends" )
		FOnFriendsRefreshed		OnFriendsRefreshed;

	/**
	 * Fires when a call to RefreshFriends errors
	 */
	UPROPERTY( BlueprintAssignable, Category = "UEOS|Friends" )
		FOnFriendsRefreshed		OnRefreshFriendsError;

private:

	static void					QueryFriendsCallback( const EOS_Friends_QueryFriendsCallbackInfo* Data );

	/** Handle for the Friends interface. */
	EOS_HFriends				FriendsHandle;
};

