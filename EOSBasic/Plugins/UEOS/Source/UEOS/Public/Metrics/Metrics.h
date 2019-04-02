// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

// Engine Includes
#include "Object.h"

// EOS Includes
#include "eos_sdk.h"
#include "eos_metrics.h"

// Plugin Includes
#include "Authentication/Authentication.h"

#include "Metrics.generated.h"

// Forward Declarations

/**
* Enum of types of supported accounts.
*/
UENUM()
enum class ESupportedAccountType : uint8
{
	SAT_Unknown			UMETA( DisplayName = "Unknown" ),
	SAT_Epic			UMETA( DisplayName = "Epic" ),
	SAT_External		UMETA( DisplayName = "External" )
};

/**
* Enum of types of Input Controller Types.
*/
UENUM()
enum class EInputControllerType : uint8
{
	ICT_Unknown			UMETA( DisplayName = "Unknown" ),
	ICT_MouseKeyboard	UMETA( DisplayName = "Mouse/Keyboard" ),
	ICT_Gamepad			UMETA( DisplayName = "Gamepad" ),
	ICT_Touch			UMETA( DisplayName = "Touch" )
};

UCLASS()
class UEOS_API UEOSMetrics : public UObject
{
	GENERATED_BODY()

public:

	/**
	* EOS Metrics Constructor.
	*/
	UEOSMetrics();

	/**
	* Initialization
	*
	* @return bool True if successfully initialised, otherwise false.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Metrics" )
		bool					InitMetric();

	/**
	* Logs metrics for a player session beginning
	*
	* @param DisplayName The name to associate with this Begin Session.  If using the Epic session, this will be taken from the Authentication.
	* @param TargetAccountType The type of account to use to register this Session.  Defaults to External.
	* @param ControllerType The type of input controller used during this play session.  Defaults to Mouse/Keyboard.
	*
	* @return bool True if successfully started the Player Session, otherwise false.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Metrics" )
		bool					BeginPlayerSession( FString DisplayName,
													ESupportedAccountType TargetAccountType = ESupportedAccountType::SAT_External,
													EInputControllerType ControllerType = EInputControllerType::ICT_MouseKeyboard );

	/**
	* Logs metrics for a player session ending
	*
	* @param DisplayName The name to associate with this End Session.  If using the Epic session, this will be taken from the Authentication.
	*
	* @return bool True if successfully ended the Player Session, otherwise false.
	*/
	UFUNCTION( BlueprintCallable, Category = "UEOS|Metrics" )
		bool					EndPlayerSession( FString DisplayName );

protected:

	/** The Account type for this Metric.  Set when BeginPlayerSession is called.  Defaults to Unknown. */
	UPROPERTY()
		ESupportedAccountType	AccountType;

private:
	
	/** Handle to EOS SDK metrics system */
	EOS_HMetrics				MetricsHandle;

};
