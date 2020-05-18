// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// UE4 Includes
#include "UObject/Object.h"

// EOS SDK Includes
#include "eos_sdk.h"
#include "eos_userinfo.h"

// EOS Pluin Includes
#include "Authentication/Authentication.h"

#include "UserInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnUserInfoRetreived, const FEpicAccountId&, EpicAccountId );

UCLASS()
class UEOS_API UEOSUserInfo : public UObject
{
	GENERATED_BODY()

public:

	UEOSUserInfo();

	/**
	 * Begins an async process to get User Info for the provided User.
	 * Broadcasts either OnUserInfoRetreived or OnUserInfoError, when the request has completed.
	 * More information can be returned for a local user.
	 *
	 * @param EpicAccountId The target Account ID to use to query for the Name.
	 */
	UFUNCTION( BlueprintCallable, Category = "UEOS|User Info" )
		void					QueryUserInfoByAccountId( const FEpicAccountId& EpicAccountId );

	/**
	 * Returns an empty string if the given AccountId has not been queried, otherwise returns
	 * the Display Name for the target account, from the cached information.
	 *
	 * @param EpicAccountId The target Epic Account ID to use to query for the Display Name.
	 */
	static FString					GetDisplayName( const FEpicAccountId& EpicAccountId );

	/**
	* Event fired when User Info has been retrieved.
	* 
	* @param AccountId The returned Account ID.
	*/
	UPROPERTY( BlueprintAssignable, Category = "UEOS|User Info" )
		FOnUserInfoRetreived	OnUserInfoRetreived;

	/**
	* Event fired when an Error has occured after attempting to retrieve User Info.
	*
	* @param AccountId The returned Account ID.
	*/
	UPROPERTY( BlueprintAssignable, Category = "UEOS|User Info" )
		FOnUserInfoRetreived	OnUserInfoError;

private:

	static void					QueryUserInfoCallback( const EOS_UserInfo_QueryUserInfoCallbackInfo* Data );

};
