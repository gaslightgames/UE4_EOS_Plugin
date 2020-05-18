// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

// Engine Includes
#include "UObject/Object.h"

// EOS Includes
#include "eos_sdk.h"
#include "eos_auth.h"
#include "eos_version.h"

#include "Authentication.generated.h"

// Forward Declarations


DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnUserLoggedIn );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnUserLoggedOut );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnUserLoginRequiresMFA );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnUserLoginFail );

/** Login Mode */
UENUM()
enum class ELoginMode : uint8
{
	LM_Unknown			UMETA( DisplayName = "Unknown" ),		/** Unknown Login Type */
	LM_IDPassword		UMETA( DisplayName = "Password" ),		/** Login using a user id and password token */
	LM_ExchangeCode		UMETA( DisplayName = "Exchange Code" ),	/** Login using an exchange code */
	LM_PinGrant			UMETA( DisplayName = "Pin Grant" ),		/** Login using a pin grant */
	LM_DevTool			UMETA( DisplayName = "Dev Tool" )		/** Login using the EOS SDK Dev Auth Tool */,
	LM_AccountPortal    UMETA(DisplayName = "Account Portal")		/** Logins using a web portal login through your browser. */,
};

/**
* Adapted from the sample, to work within UE4 UBT.
*/
USTRUCT(BlueprintType)
struct UEOS_API FEpicAccountId
{
	GENERATED_BODY()

	/**
	* Construct wrapper from account id.
	*/
	FEpicAccountId(EOS_EpicAccountId InAccountId)
	: EpicAccountId(InAccountId)
	{
		AccountIdAsString = ToString();
	};

	FEpicAccountId() = default;

	FEpicAccountId(const FEpicAccountId&) = default;

	FEpicAccountId& operator=(const FEpicAccountId&) = default;

	bool operator==(const FEpicAccountId& Other) const
	{
		return EpicAccountId == Other.EpicAccountId;
	}

	bool operator!=(const FEpicAccountId& Other) const
	{
		return !(this->operator==(Other));
	}

	bool operator<(const FEpicAccountId& Other) const
	{
		return EpicAccountId < Other.EpicAccountId;
	}

	/**
	* Checks if account ID is valid.
	*/
	operator bool() const;

	/**
	* Easy conversion to EOS account ID.
	*/
	operator EOS_EpicAccountId() const
	{
		return EpicAccountId;
	}

	UPROPERTY(BlueprintReadOnly)
		FString AccountIdAsString;
	
	/**
	* Prints out account ID as hex.
	*/
	FString						ToString() const;

	/**
	* Returns an Account ID from a String interpretation of one.
	*
	* @param AccountId the FString representation of an Account ID.
	* @return FEpicAccountId An Account ID from the string, if valid.
	*/
	static FEpicAccountId		FromString(const FString& AccountId);

	/** The EOS SDK matching Account Id. */
	EOS_EpicAccountId			EpicAccountId;

};


UCLASS()
class UEOS_API UEOSAuthentication : public UObject
{
	GENERATED_BODY()

public:

	/**
	* EOS Authentication Constructor.
	*/
	UEOSAuthentication();

	/**
	* Attempts to login user with given credentials
	*
	* @param LoginMode - Login mode
	* @param UserId - Id of the user logging in
	* @param UserToken - Credentials or token related to the user logging in
	*/
	UFUNCTION(BlueprintCallable, Category = "UEOS|Authentication")
		void						Login(ELoginMode LoginMode, FString UserId, FString UserToken);

	/**
	* Logs current user out
	*/
	UFUNCTION(BlueprintCallable, Category = "UEOS|Authentication")
		void						Logout();

	/**
	* Whether this Account has been authorised.
	*/
	UFUNCTION(BlueprintCallable, Category = "UEOS|Authentication")
		bool						GetAuthorised();

	/**
	* Creates an auth token for use elsewhere (for example, on a server).
	* Must be cleaned up by calling ReleaseAuthToken when you're done with it.
	*
	* @param OutToken A copy of the Auth Token.
	* @return bool true on succcess, false on failure.
	*/
	bool							GetAuthTokenCopy(EOS_Auth_Token** OutToken);

	/**
	* Cleans up memory that had been allocated in GetAuthTokenCopy
	*
	* @param Token The Auth Token to clean-up/releae.
	*/
	void							ReleaseAuthToken(EOS_Auth_Token* Token);

	/**
	* Utility to convert account id to a string
	*
	* @param InAccountId - Account id to convert
	* @return FString representing the account ID.
	*/
	static FString					AccountIDToString(EOS_EpicAccountId InAccountId);

	/**
	* Fires when a User Has Logged In.
	*/
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Authentication")
		FOnUserLoggedIn				OnUserLoggedIn;

	/**
	* Fires when a User Has Logged Out.
	*/
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Authentication")
		FOnUserLoggedOut			OnUserLoggedOut;

	/**
	* Fires when a Login Attempt requres Multi-Factor Authentication.
	*/
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Authentication")
		FOnUserLoginRequiresMFA		OnUserLoginRequiresMFA;

	/**
	* Fires when a Login Attempt Fails.
	*/
	UPROPERTY(BlueprintAssignable, Category = "UEOS|Authentication")
		FOnUserLoginFail			OnUserLoginFail;

	UFUNCTION(BlueprintCallable, Category = "UEOS|Authentication")
		FEpicAccountId GetEpicAccountId() const
	{
		return EpicAccountId;
	}

protected:


private:

	/**
	* Callback that is fired when the login operation completes, either successfully or in error
	*
	* @param Data - Output parameters for the EOS_Auth_Login Function
	*/
	static void						LoginCompleteCallback(const EOS_Auth_LoginCallbackInfo* Data);

	/**
	* Callback that is fired when the logout operation completes, either successfully or in error
	*
	* @param Data - Output parameters for the EOS_Auth_Logout Function
	*/
	static void						LogoutCompleteCallback(const EOS_Auth_LogoutCallbackInfo* Data);

	/**
	* Utility for printing auth token info.
	*
	* @param EOS_Auth_Token* The Auth Token to print.
	*/
	static void						PrintAuthToken(EOS_Auth_Token* InAuthToken);

	/** Handle for Auth interface. */
	EOS_HAuth						AuthHandle;

	/** The Epic Account ID for this Authentication session. */
	FEpicAccountId					EpicAccountId;

	/** Whether a Login has been successful and this Authentication session is Authorised. */
	UPROPERTY()
		bool						bAuthorised;
};
