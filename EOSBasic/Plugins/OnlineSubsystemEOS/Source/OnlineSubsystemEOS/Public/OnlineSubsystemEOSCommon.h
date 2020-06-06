// Copyright (C) Gaslight Games Ltd, 2019-2020

#pragma once

// EOS Includes
#include "eos_sdk.h"
#include "eos_logging.h"


UENUM( BlueprintType )
enum class EEOSResultType : uint8
{
	RT_Result						UMETA( DisplayName = "Result" ),
	RT_Auth							UMETA( DisplayName = "Auth" ),
	RT_Friends						UMETA( DisplayName = "Friends" ),
	RT_Presence						UMETA( DisplayName = "Presence" ),
	RT_Ecom							UMETA( DisplayName = "Ecom" ),
	RT_Unknown						UMETA( DisplayName = "Unknown" )
};

// Enum of EOS Results
UENUM( BlueprintType )
enum class EEOSResults : uint8
{
	ER_Success						UMETA( DisplayName = "Success" ),

	ER_AlreadyInitialized			UMETA( DisplayName = "Already Initialized" ),
	ER_AlreadyShutdown				UMETA( DisplayName = "Already Shutdown" ),
	ER_NotInitialized				UMETA( DisplayName = "Not Initialized" ),
	ER_PlatformFailed				UMETA( DisplayName = "Platform Handle Failed" ),

	ER_NoConnection					UMETA( DisplayName = "No Connection" ),
	ER_InvalidCredentials			UMETA( DisplayName = "Invalid Credentials" ),
	ER_InvalidUser					UMETA( DisplayName = "Invalid User" ),
	ER_InvalidAuth					UMETA( DisplayName = "Invalid Auth" ),
	ER_AccessDenied					UMETA( DisplayName = "Access Denied" ),
	ER_MissingPermissions			UMETA( DisplayName = "Missing Permissions" ),
	ER_TokenNotAccount				UMETA( DisplayName = "Token Not Account" ),
	ER_TooManyRequests				UMETA( DisplayName = "Too Many Requests" ),
	ER_AlreadyPending				UMETA( DisplayName = "Already Pending" ),
	ER_InvalidParameters			UMETA( DisplayName = "Invalid Parameters" ),
	ER_InvalidRequest				UMETA( DisplayName = "Invalid Request" ),
	ER_UnrecognizedResponse			UMETA( DisplayName = "Unrecognized Response" ),
	ER_IncompatibleVersion			UMETA( DisplayName = "Incompatible Version" ),
	ER_NotConfigured				UMETA( DisplayName = "Not Configured" ),
	ER_AlreadyConfigured			UMETA( DisplayName = "Already Configured" ),
	ER_NotImplemented				UMETA( DisplayName = "Feature Not Implemented" ),
	ER_Canceled						UMETA( DisplayName = "Canceled" ),
	ER_NotFound						UMETA( DisplayName = "Not Found" ),
	ER_OperationWillRetry			UMETA( DisplayName = "Operation Will Retry" ),
	ER_NoChange						UMETA( DisplayName = "No Change" ),
	ER_VersionMismatch				UMETA( DisplayName = "Version Mismatch" ),
	ER_LimitExceeded				UMETA( DisplayName = "Limit Exceeded" ),
	ER_Disabled						UMETA( DisplayName = "Disabled" ),
	ER_DuplicateNotAllowed			UMETA( DisplayName = "Duplicate Not Allowed" ),
	ER_MissingParameters			UMETA( DisplayName = "Missing Parameters" ),
	ER_UnknownError					UMETA( DisplayName = "Unknown Error" )
};

// Enum of EOS Auth Responses
UENUM( BlueprintType )
enum class EEOSAuth : uint8
{
	EA_UnknownError					UMETA( DisplayName = "Unknown Error" )
};

// Enum of EOS Friends Responses
UENUM( BlueprintType )
enum class EEOSFriends : uint8
{
	EF_UnknownError					UMETA( DisplayName = "Unknown Error" )
};

// Enum of EOS Presence Responses
UENUM( BlueprintType )
enum class EEOSPresence : uint8
{
	EP_UnknownError					UMETA( DisplayName = "Unknown Error" )
};

// Enum of EOS Auth Responses
UENUM( BlueprintType )
enum class EEOSEcom : uint8
{
	EE_UnknownError					UMETA( DisplayName = "Unknown Error" )
};


class UEOSCommon
{
public:

	/**
	* Converts between an EOS SDK result Enum, to a Blueprint friendly enum.
	* As UENUMs must currently be uint8, we're limited to 255 values per enum, so we cannot do a direct
	* equivalence between the SDK values and some UENUM.  Instead, we have separate enums for each
	* section and this function converts between them.
	*
	* @param SDKResult The Enum FROM the SDK.
	* @param Result If the SDK Enum is in this range, this will be populated. Otherwise ER_UnknownError.
	* @param Auth If the SDK Enum is in this range, this will be populated. Otherwise EA_UnknownError.
	* @param Friends If the SDK Enum is in this range, this will be populated. Otherwise EF_UnknownError.
	* @param Presence If the SDK Enum is in this range, this will be populated. Otherwise EP_UnknownError.
	* @param Ecom If the SDK Enum is in this range, this will be populated. Otherwise EE_UnknownError.
	*
	* @return EEOSResultType Which kind of result has been populated.
	*/
	static EEOSResultType			GetUnrealFriendlyResult( EOS_EResult SDKResult, EEOSResults& Result, EEOSAuth& Auth, EEOSFriends& Friends, EEOSPresence& Presence, EEOSEcom& Ecom );

	/**
	* Get the EOS Plugin version of the results, from the SDK Enum.
	* The EOS side is Engine/Blueprint ready.
	*
	* @param SDKResult The SDK Enum to request turning into a BP version.
	* @return EEOSResults The EOS Plugin, Engine/Blueprint version.
	*/
	static EEOSResults				GetResultsValue( EOS_EResult SDKResult );

	static EEOSAuth					GetAuthValue( EOS_EResult SDKResult );

	static EEOSFriends				GetFriendsValue( EOS_EResult SDKResult );

	static EEOSPresence				GetPresenceValue( EOS_EResult SDKResult );

	static EEOSEcom					GetEcomValue( EOS_EResult SDKResult );

	/**
	* Utility to return an EOS Result as a FString.
	*
	* @param Result The EOS Result to attempt to convert.
	* @return FString result of the conversion.
	*/
	static FString					EOSResultToString( EOS_EResult Result );
};
