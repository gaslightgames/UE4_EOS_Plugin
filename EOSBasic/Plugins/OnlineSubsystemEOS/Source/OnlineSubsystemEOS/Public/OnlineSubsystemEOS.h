// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

#include "CoreMinimal.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystemImpl.h"

// EOS Includes
#include "eos_sdk.h"

// Forward Declarations
class FOnlineIdentityEOS;

/** Forward declarations of all interface classes */
typedef TSharedPtr<FOnlineIdentityEOS, ESPMode::ThreadSafe> FOnlineIdentityEOSPtr;


// Subsystem Name
#ifndef EOS_SUBSYSTEM
#define EOS_SUBSYSTEM FName( TEXT( "EOS" ) )
#endif

/**
 *	OnlineSubsystemSteam - Implementation of the online subsystem for EPIC ONLINE SERVICES (EOS)
 */
class ONLINESUBSYSTEMEOS_API FOnlineSubsystemEOS : public FOnlineSubsystemImpl
{

public:

	virtual ~FOnlineSubsystemEOS()
	{
	}

	// IOnlineSubsystem
	virtual IOnlineSessionPtr			GetSessionInterface() const override;
	virtual IOnlineFriendsPtr			GetFriendsInterface() const override;
	virtual IOnlinePartyPtr				GetPartyInterface() const override;
	virtual IOnlineGroupsPtr			GetGroupsInterface() const override;
	virtual IOnlineSharedCloudPtr		GetSharedCloudInterface() const override;
	virtual IOnlineUserCloudPtr			GetUserCloudInterface() const override;
	virtual IOnlineLeaderboardsPtr		GetLeaderboardsInterface() const override;
	virtual IOnlineVoicePtr				GetVoiceInterface() const override;
	virtual IOnlineExternalUIPtr		GetExternalUIInterface() const override;
	virtual IOnlineTimePtr				GetTimeInterface() const override;
	virtual IOnlineIdentityPtr			GetIdentityInterface() const override;
	virtual IOnlineTitleFilePtr			GetTitleFileInterface() const override;
	virtual IOnlineEntitlementsPtr		GetEntitlementsInterface() const override;
	virtual IOnlineStorePtr				GetStoreInterface() const override;
	virtual IOnlineStoreV2Ptr			GetStoreV2Interface() const override;
	virtual IOnlinePurchasePtr			GetPurchaseInterface() const override;
	virtual IOnlineEventsPtr			GetEventsInterface() const override;
	virtual IOnlineAchievementsPtr		GetAchievementsInterface() const override;
	virtual IOnlineSharingPtr			GetSharingInterface() const override;
	virtual IOnlineUserPtr				GetUserInterface() const override;
	virtual IOnlineMessagePtr			GetMessageInterface() const override;
	virtual IOnlinePresencePtr			GetPresenceInterface() const override;
	virtual IOnlineChatPtr				GetChatInterface() const override;
	virtual IOnlineStatsPtr				GetStatsInterface() const override;
	virtual IOnlineTurnBasedPtr			GetTurnBasedInterface() const override;
	virtual IOnlineTournamentPtr		GetTournamentInterface() const override;
	virtual bool						IsLocalPlayer( const FUniqueNetId& UniqueId ) const override;
	virtual bool						Init() override;
	virtual bool						Shutdown() override;
	virtual bool						Exec( class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar ) override;
	virtual bool						IsEnabled() const override;
	virtual FString						GetAppId() const override;
	virtual FText						GetOnlineServiceName() const override;

	// FTickerObjectBase
	virtual bool						Tick( float DeltaTime ) override;

	/**
	* Checks whether the EOS SDK has been Initialized.
	*
	* @return bool True if has been initialized, otherwise false.
	*/
	bool								IsEOSInitialized() { return bEOSInitialized; };

	/**
	* Returns the current Platform Handle.
	* Can be NULL if the SDK has not been initialized or failed to initialize.
	*
	* @return EOS_HPlatform The current EOS Platform Handle.
	*/
	EOS_HPlatform						GetPlatformHandle() { return PlatformHandle; };

protected:

	// Attempt to gather the Config Options for the EOS
	bool								GetEOSConfigOptions();

	// Attempt to Initialize the SDK
	bool								InitializeSDK();

	// Attempt to Create a valid Platform Handle from the SDK
	bool								CreatePlatformHandle();


	/** The Product Name for the running game. */
	FString								ProductName;

	/** The Product Version for the running game. */
	FString								ProductVersion;

	/** The ProductID for the running game. */
	FString								ProductId;

	/** The SandboxID for the running game. */
	FString								SandboxId;

	/** The DeploymentId for the running game. */
	FString								DeploymentId;

	/** The ClientID for the running game. */
	FString								ClientId;

	/** The ClientSecret for the running game. */
	FString								ClientSecret;

	/// ---------------------------------------------------
	/// Subsystem Interfaces
	/// ---------------------------------------------------

	/** Interface to the profile services */
	FOnlineIdentityEOSPtr				IdentityInterface;

	/// ---------------------------------------------------

PACKAGE_SCOPE :

	/** Only the factory makes instances */
	FOnlineSubsystemEOS() = delete;
	FOnlineSubsystemEOS( FName InInstanceName )
		: FOnlineSubsystemImpl( EOS_SUBSYSTEM, InInstanceName )
		, ProductName( "" )
		, ProductVersion( "" )
		, ProductId( "" )
		, SandboxId( "" )
		, DeploymentId( "" )
		, ClientId( "" )
		, ClientSecret( "" )
		, IdentityInterface( nullptr )
		, bEOSInitialized( false )
		, PlatformHandle( nullptr )
	{}

private:

	bool								bEOSInitialized;

	/** The EOS Platform Handle for Platform operations. */
	EOS_HPlatform						PlatformHandle;

};

typedef TSharedPtr<FOnlineSubsystemEOS, ESPMode::ThreadSafe> FOnlineSubsystemEOSPtr;
