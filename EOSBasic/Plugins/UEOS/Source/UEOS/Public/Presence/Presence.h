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
	Offline = 0,
	Online = 1,
	Away = 2,
	ExtendedAway = 3,
	DoNotDisturb = 4
};

UCLASS()
class UEOS_API UEOSPresence : public UObject
{
	GENERATED_BODY()

public:

	UEOSPresence();

	UFUNCTION(BlueprintCallable, Category = "UEOS|Presence")
		EPresenceStatus GetPresenceStatus(const FEpicAccountId& InFriendInfo);

	UFUNCTION(BlueprintCallable, Category = "UEOS|Presence")
	void AddFriendPresence(const FEpicAccountId& InFriendInfo);

	/**
	 * Begins an async process that requests the presence statuses of the friends.
	 */
	UFUNCTION(BlueprintCallable, Category = "UEOS|Presence")
		void RequestFriendPresences();

protected:

	static void SetPresenceCallback(const EOS_Presence_SetPresenceCallbackInfo* Data);

	static void QueryPresenceCompleteCallback(const EOS_Presence_QueryPresenceCallbackInfo* Data);
	static void OnPresenceChangedCallback(const EOS_Presence_PresenceChangedCallbackInfo* Data);

	static void SetPresence(FEpicAccountId TargetUserId);

};

