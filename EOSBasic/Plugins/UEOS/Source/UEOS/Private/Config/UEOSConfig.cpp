// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "Config/UEOSConfig.h"

UEOSConfig::UEOSConfig()
	: ProductName( "EOS Plugin" )
	, ProductVersion( "1.0" )
	, ProductId( "" )
	, SandboxId( "" )
	, DeploymentId( "" )
	, ClientId( "" )
	, ClientSecret( "" )
	, bIsServer( false )
	, LogLevel( ELogLevel::LL_VeryVerbose )
{

}
