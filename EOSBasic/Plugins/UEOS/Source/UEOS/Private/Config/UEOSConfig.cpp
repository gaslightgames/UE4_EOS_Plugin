// Copyright (C) Gaslight Games Ltd, 2019

#include "Public/Config/UEOSConfig.h"

UEOSConfig::UEOSConfig()
	: ProductId( "" )
	, SandboxId( "" )
	, ClientId( "" )
	, ClientSecret( "" )
	, bIsServer( false )
	, bAllowReservedPlatforms( false )
	, LogLevel( ELogLevel::LL_VeryVerbose )
{

}

void UEOSConfig::SetProductId( FString NewProductId )
{
	ProductId = NewProductId;
	SaveConfig();
}

void UEOSConfig::SetSandboxId( FString NewSandboxId )
{
	SandboxId = NewSandboxId;
	SaveConfig();
}

void UEOSConfig::SetClientId( FString NewClientId )
{
	ClientId = NewClientId;
	SaveConfig();
}

void UEOSConfig::SetClientSecret( FString NewClientSecret )
{
	ClientSecret = NewClientSecret;
	SaveConfig();
}

void UEOSConfig::SetIsServer( bool bServer )
{
	if( bIsServer != bServer )
	{
		bIsServer = bServer;
		SaveConfig();
	}
}

void UEOSConfig::SetAllowReservedPlatforms( bool bAllow )
{
	if( bAllowReservedPlatforms != bAllow )
	{
		bAllowReservedPlatforms = bAllow;
		SaveConfig();
	}
}

void UEOSConfig::SetLogLevel( ELogLevel NewLogLevel )
{
	LogLevel = NewLogLevel;
	SaveConfig();
}
