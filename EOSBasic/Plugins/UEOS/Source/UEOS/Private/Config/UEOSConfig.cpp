// Copyright (C) Gaslight Games Ltd, 2019

#include "Public/Config/UEOSConfig.h"

UEOSConfig::UEOSConfig()
	: ProductName( "EOS Plugin" )
	, ProductVersion( "1.0" )
	, ProductId( "" )
	, SandboxId( "" )
	, ClientId( "" )
	, ClientSecret( "" )
	, bIsServer( false )
	, bAllowReservedPlatforms( false )
	, LogLevel( ELogLevel::LL_VeryVerbose )
{

}
