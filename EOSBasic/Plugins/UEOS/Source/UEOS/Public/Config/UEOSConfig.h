// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

// Engine Includes
#include "Object.h"
#include "Engine/DeveloperSettings.h"

#include "UEOSConfig.generated.h"

UENUM()
enum class ELogLevel : uint8
{
	LL_Off				UMETA( DisplayName = "Off" ),			/* EOS_LOG_Off */
	LL_Fatal			UMETA( DisplayName = "Fatal" ),			/* EOS_LOG_Fatal */
	LL_Error			UMETA( DisplayName = "Error" ),			/* EOS_LOG_Error */
	LL_Warning			UMETA( DisplayName = "Warning" ),		/* EOS_LOG_Warning */
	LL_Info				UMETA( DisplayName = "Info" ),			/* EOS_LOG_Info */
	LL_Verbose			UMETA( DisplayName = "Verbose" ),		/* EOS_LOG_Verbose */
	LL_VeryVerbose		UMETA( DisplayName = "Very Verbose" ),	/* EOS_LOG_VeryVerbose */
};

/**
* Config class for EOS settings.
* Allows for the settings to be displayed in the Project Settings.
* Also saves settings to a file.
*/
UCLASS( config = Game, defaultconfig, meta = ( DisplayName = "UEOS" ) )
class UEOS_API UEOSConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UEOSConfig();

	/** Sets the Product Name. */
	void			SetProductName( FString NewProductName );

	/** Sets the Product Version. */
	void			SetProductVersion( FString NewProductVersion );

	/** Sets the ProductID. */
	void			SetProductId( FString NewProductId );

	/** Sets the SandboxID. */
	void			SetSandboxId( FString NewSandboxId );

	/** Sets the ClientID. */
	void			SetClientId( FString NewClientId );

	/** Sets the ClientSecret. */
	void			SetClientSecret( FString NewClientSecret );

	/** Sets whether this is a Server for EOS, or not. */
	void			SetIsServer( bool bServer );

	/** Sets whether EOS uses Reserved Platforms or not. */
	void			SetAllowReservedPlatforms( bool bAllow );

	/** Sets the Target Log Level. */
	void			SetLogLevel( ELogLevel NewLogLevel );

	/** The Product Name to pass to EOS for this Project. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		FString		ProductName;

	/** The Product Version to pass to EOS for this Project. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		FString		ProductVersion;

	/** The ProductID to pass to EOS for this Project. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		FString		ProductId;

	/** The SandboxID to pass to EOS for this Project. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		FString		SandboxId;

	/** The ClientID to pass to EOS for this Project. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		FString		ClientId;

	/** The ClientSecret to pass to EOS for this Project. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		FString		ClientSecret;

	/** Whether this EOS instance is a Server or not.  Defaults to false. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		bool		bIsServer;

	/** Whether to allow reserved platforms.  Defaults to false. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		bool		bAllowReservedPlatforms;

	/** The Log Level.  Defaults to Very Verbose. */
	UPROPERTY( config, EditAnywhere, BlueprintReadWrite, Category = "UEOS" )
		ELogLevel	LogLevel;
};
