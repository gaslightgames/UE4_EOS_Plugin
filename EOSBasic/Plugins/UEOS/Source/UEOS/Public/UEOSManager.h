// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

// Engine Includes
#include "Object.h"

// EOS Includes
#include "eos_sdk.h"
#include "eos_logging.h"

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

protected:

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		bool									InitEOS();

	UFUNCTION( BlueprintCallable, Category = "UEOS|Manager" )
		bool									ShutdownEOS();

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
