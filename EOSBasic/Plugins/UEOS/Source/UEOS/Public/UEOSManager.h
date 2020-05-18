// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// EOS Includes
#include "UEOSCommon.h"
#include "Config/UEOSConfig.h"
#include "Presence/Presence.h"

#include "UEOSManager.generated.h"

// Forward Declarations
class UEOSAuthentication;
class UEOSMetrics;
class UEOSFriends;
class UEOSUserInfo;

UCLASS(BlueprintType)
class UEOS_API UEOSManager : public UObject
{
	GENERATED_BODY()

public:

	/**
	* EOS Manager Constructor.
	*/
	UEOSManager();

	/**
	* Request the EOS Manager.
	* EOS cannot run within PIE (Play-in-Editor). Ensure to check the
	* boolean flag before trying to access the EOS Manager object.
	*
	* @param ActiveEOSManager The current instance of the EOS Manager.
	* @return bool True if the Manager has been returned, otherwise false.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager", meta = ( WorldContext = WorldContextObject ) )
		static bool								RequestEOSManager( UEOSManager*& ActiveEOSManager, UObject* WorldContextObject );

	/**
	* Use this class as a Singleton and thus, returns the current instance.
	* @return UEOSManager The singleton active instance.
	*/
	static UEOSManager*							GetEOSManager();

	/**
	* Cleans up all content within the Singleton.
	* Requests a delete of the current UEOSManager instance.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static void								Cleanup();

	/**
	* Whether the EOS System has been successfully initialized or not.
	*
	* @return bool True if initialized, otherwise false.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static bool								IsEOSInitialized();

	/**
	* Whether the EOS System has been shutdown or not.
	* If it HAS been shutdown in this session, no other calls (not even Initialization)
	* can be made without an application restart.
	*
	* @return bool True if initialized, otherwise false.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static bool								HasEOSBeenShutdown();

	/**
	* Attempts to return the current Platform Handle.
	*
	* @return EOS_HPlatform The current Platform Handle.
	*/
	static EOS_HPlatform						GetPlatformHandle();

	/**
	* Attempts to return the current Authentication object.
	*
	* @return UEOSAuthentication* The current Authentication object, or nullptr if not valid.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static UEOSAuthentication*				GetAuthentication();

	/**
	* Attempts to return the current Metrics object.
	*
	* @return UEOSMetrics* The current Metrics object, or nullptr if not valid.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static UEOSMetrics*						GetMetrics();

	/**
	 * Attempts to return the current Friends object.
	 *
	 * @return UEOSFriends* The current Friends object, or nullptr if not valid.
	 */
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static UEOSFriends*						GetFriends();

	/**
	 * Attempts to return the current UserInfo object.
	 *
	 * @return UEOSUserInfo* The current UserInfo object, or nullptr if not valid.
	 */
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static UEOSUserInfo*					GetUserInfo();


	/**
	 * Attempts to return the current Presence object.
	*
	* @return The current Presence object to return, or nullptr if not valid.
	*/

	UFUNCTION(BlueprintCallable, Category = "UEOS|Manager")
		static UEOSPresence* GetPresence();


	/// CONFIG GETTERS

	/**
	* Returns the ProductID used in this project.
	*
	* @return FString The ProductID that passed to EOS for this Project.
	*/
	UFUNCTION( BlueprintPure, Category = "UEOS", meta = ( Keywords = "Get Product Id" ) )
		static FString							GetProductId();

	/**
	* Returns the SandboxID used in this project.
	*
	* @return FString The SandboxID that passed to EOS for this Project.
	*/
	UFUNCTION( BlueprintPure, Category = "UEOS", meta = ( Keywords = "Get Sandbox Id" ) )
		static FString							GetSandboxId();

	/**
	* Returns the Support Ticketing authentication key used in this project.
	*
	* @return FString The authentication key that is passed to the support ticket mailbox.
	*/
	UFUNCTION( BlueprintPure, Category = "UEOS", meta = ( Keywords = "Get Support Ticketing Key" ) )
		static FString							GetSupportTicketingKey();

	/**
	* Returns the The URL to send EOS support tickets to.
	*
	* @return FString The URL that is used to send support tickets.
	*/
	UFUNCTION( BlueprintPure, Category = "UEOS", meta = ( Keywords = "Get Support Ticketing URL" ) )
		static FString							GetSupportTicketingURL();

	/**
	* Returns the ClientID used in this project.
	*
	* @return FString The ClientID that passed to EOS for this Project.
	*/
	UFUNCTION( BlueprintPure, Category = "UEOS", meta = ( Keywords = "Get Client Id" ) )
		static FString							GetClientId();

	/**
	* Returns the ClientSecret used in this project.
	*
	* @return FString The ClientSecret that passed to EOS for this Project.
	*/
	UFUNCTION( BlueprintPure, Category = "UEOS", meta = ( Keywords = "Get Client Secret" ) )
		static FString							GetClientSecret();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		EEOSResults					  			InitEOS();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		EEOSResults						  		ShutdownEOS();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		bool							       	UpdateEOS();


protected:

	// --------------------------------------------------------------
	// STATIC PROPERTIES
	// --------------------------------------------------------------

	/** The singleton UEOSManager instance. */
	static UEOSManager*							EOSManager;

	// --------------------------------------------------------------
	// INSTANCE PROPERTIES
	// --------------------------------------------------------------

	/** The EOS Platform Handle for Platform operations. */
	EOS_HPlatform							  	PlatformHandle;

	/** Whether the EOS System has been initialized. */
	UPROPERTY()
		bool									bEOSInitialized;

	/** Whether the EOS System has been Shutdown.  If it has, it CANNOT be reinitialized without a full
	 * application restart, as any further calls would result in undefined behaviour. */
	UPROPERTY()
		bool									bEOSShutdown;

	/** The current Presence object. */
	UPROPERTY()
		UEOSPresence* Presence;
	
	/** The current Authentication object. */
	UPROPERTY()
		UEOSAuthentication*						Authentication;

	/** The current Metric object. */
	UPROPERTY()
		UEOSMetrics*							Metrics;

	/** The current Friends object. */
	UPROPERTY()
		UEOSFriends*							Friends;

	/** The current UserInfo object. */
	UPROPERTY()
		UEOSUserInfo*							UserInfo;

protected:

	/* Whether or not we want to read from the Config directory or read from text files of the secret keys. */
		bool									bUseConfig;

private:

	/**
	* Callback function to use for EOS SDK log messages
	*
	* @param InMsg - A structure representing data for a log message
	*/
	static void									EOSSDKLoggingCallback( const EOS_LogMessage* InMsg );

};
