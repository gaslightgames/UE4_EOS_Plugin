#pragma once

// UE4 Includes
#include "Object.h"

// EOS SDK Includes
#include "eos_sdk.h"
#include "eos_userinfo.h"

// EOS Pluin Includes
#include "Authentication/Authentication.h"

#include "UserInfo.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnUserInfoRetreived, const FAccountId&, AccountId );

UCLASS()
class UEOS_API UEOSUserInfo : public UObject
{
	GENERATED_BODY()

public:

	UEOSUserInfo();

	void Init();

	/**
	 * Begins an async process to get this user's display name.  Broadcasts either OnUserInfoRetreived or OnUserInfoError
	 * when the request has completed.
	 *
	 * @param AccountId The target Account ID to use to query for the Name.
	 */
	void						QueryNameByAccountId( const FAccountId& AccountId );

	/**
	 * Returns an empty string if the given AccountId has not been queried.
	 *
	 * @param AccountId The target Account ID to use to query for the Display Name.
	 */
	FString						GetDisplayName( const FAccountId& AccountId );

	/**
	* Event fired when User Info has been retrieved.
	* 
	* @param AccountId The returned Account ID.
	*/
	FOnUserInfoRetreived		OnUserInfoRetreived;

	/**
	* Event fired when an Error has occured after attempting to retrieve User Info.
	*
	* @param AccountId The returned Account ID.
	*/
	FOnUserInfoRetreived		OnUserInfoError;

private:

	static void					QueryUserInfoCallback( const EOS_UserInfo_QueryUserInfoCallbackInfo* Data );

	/** Handle for the UserInfo interface */
	EOS_HUserInfo				UserInfoHandle;
};
