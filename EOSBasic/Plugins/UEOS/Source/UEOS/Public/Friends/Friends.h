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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendInviteSent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendInviteRejected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendInviteAccepted);

UCLASS()
class UEOS_API UEOSFriends : public UObject
{
	GENERATED_BODY()

public:

	UEOSFriends();

	/**
	 * Begins an async process that requests the friends count, account ids, and statuses of the local player's friends.
	 * Broadcasts OnFriendsRefreshed when completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		void						QueryFriends();

	/**
	 * Gets the current friends count on the account you are associated with.
	 * NOTE: Your friends must be added to your account on the Epic Games product list.
	 */
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		int32							GetFriendsCount();

	/**
	 * Request a specific friend index.
	 *
	 * @param Index - index of player on the friends list callback
	 */
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		void					RequestFriendId( int32 Index );

	//TODO - Test the following three functions
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		void SendInvite(const FEpicAccountId& FriendInfo);
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		void AcceptInvite(const FEpicAccountId& FriendInfo);
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		void RejectInvite(const FEpicAccountId& FriendInfo);

	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		void AddNotifyFriendsHandle();
	
	UFUNCTION(BlueprintCallable, Category = "UEOS|Friends")
		EFriendStatus GetStatus(const FEpicAccountId& FriendInfo);

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

	/**
	 * Fires when a call to SendInvite succeeds
	 */
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Friends")
		FOnFriendInviteSent		OnFriendInviteSent;

	/**
	 * Fires when a call to SendInvite errors
	 */
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Friends")
		FOnFriendInviteSent		OnFriendSendInviteError;
	
	/**
	 * Fires when a call to AcceptInvite succeeds
	 */
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Friends")
		FOnFriendInviteAccepted		OnFriendInviteAccepted;

	/**
	 * Fires when a call to AcceptInvite errors
	 */
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Friends")
		FOnFriendInviteAccepted		OnFriendAcceptInviteError;

	/**
	 * Fires when a call to RejectInvite succeeds
	 */
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Friends")
		FOnFriendInviteRejected		OnFriendInviteRejected;

	/**
	 * Fires when a call to RejectInvite errors
	 */
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Friends")
		FOnFriendInviteRejected		OnFriendRejectedError;

private:

	static void OnFriendsUpdateCallback(const EOS_Friends_OnFriendsUpdateInfo* Data);

	static void	QueryFriendsCallback( const EOS_Friends_QueryFriendsCallbackInfo* Data );
	
	static void SendInviteCallback(const EOS_Friends_SendInviteCallbackInfo* Data);
	static void AcceptInviteCallback(const EOS_Friends_AcceptInviteCallbackInfo* Data);
	static void RejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data);

	
};

