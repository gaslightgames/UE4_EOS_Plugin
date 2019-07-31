// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

// EOS Includes
#include "UEOSCommon.h"

#include "UEOSManager.generated.h"

// Forward Declarations
class UEOSAuthentication;
class UEOSMetrics;


UCLASS()
class UEOS_API UEOSManager : public UObject
{
	GENERATED_BODY()

public:

	/**
	* EOS Manager Constructor.
	*/
	UEOSManager();

	/**
	* Use this class as a Singleton and thus, returns the current instance.
	* @return UEOSManager The singleton active instance.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		static UEOSManager*						GetEOSManager();

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
	* Utility to return an EOS Result as a FString.
	*
	* @param Result The EOS Result to attempt to convert.
	* @return FString result of the conversion.
	*/
	static FString								EOSResultToString( EOS_EResult Result );

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

protected:

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		EEOSResults								InitEOS();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		EEOSResults								ShutdownEOS();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		bool									UpdateEOS();

	// --------------------------------------------------------------
	// STATIC PROPERTIES
	// --------------------------------------------------------------

	/** The singleton UEOSManager instance. */
	static UEOSManager*							EOSManager;

	// --------------------------------------------------------------
	// INSTANCE PROPERTIES
	// --------------------------------------------------------------

	/** The EOS Platform Handle for Platform operations. */
	EOS_HPlatform								PlatformHandle;

	/** Whether the EOS System has been initialized. */
	UPROPERTY()
		bool									bEOSInitialized;

	/** Whether the EOS System has been Shutdown.  If it has, it CANNOT be reinitialized without a full
	 * application restart, as any further calls would result in undefined behaviour. */
	UPROPERTY()
		bool									bEOSShutdown;

	/** The current Authentication object. */
	UPROPERTY()
		UEOSAuthentication*						Authentication;

	/** The current Metric object. */
	UPROPERTY()
		UEOSMetrics*							Metrics;

private:

	/**
	* Callback function to use for EOS SDK log messages
	*
	* @param InMsg - A structure representing data for a log message
	*/
	static void									EOSSDKLoggingCallback( const EOS_LogMessage* InMsg );
};
