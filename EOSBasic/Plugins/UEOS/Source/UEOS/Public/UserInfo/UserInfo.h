#pragma once

#include "Object.h"

#include "eos_sdk.h"
#include "eos_userinfo.h"

#include "UserInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserInfoRetreived, const FAccountId&, AccountId);

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
	 */
	void QueryNameByAccountId(const FAccountId& AccountId);
	/**
	 * Returns an empty string if the given AccountId has not been queried.
	 */
	FString GetDisplayName(const FAccountId& AccountId);

	FOnUserInfoRetreived OnUserInfoRetreived;
	FOnUserInfoRetreived OnUserInfoError;

private:
	static void QueryUserInfoCallback(const EOS_UserInfo_QueryUserInfoCallbackInfo* Data);

	/** Handle for the UserInfo interface */
	EOS_HUserInfo UserInfoHandle;
};
