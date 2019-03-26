// Copyright (C) Gaslight Games Ltd, 2019

#pragma once

#include "Object.h"
#include "Engine/DeveloperSettings.h"
#include "UEOSConfig.generated.h"

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

	/** Returns whether the EOS is currently enabled or not. */
	bool			GetEnableEOS();

	/** Sets whether EOS is enabled or disabled. */
	void			SetEnableEOS( bool bEnabled );

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

	/** Whether EOS is enabled at all.  Defaults to true. */
	UPROPERTY( config, EditAnywhere, Category = "UEOS" )
		bool		bEnableEOS;

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
};
