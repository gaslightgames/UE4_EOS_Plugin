// Copyright (C) Gaslight Games Ltd, 2019-2020

#include "Config/UEOSConfig.h"
#include "TextReaderComponent.h"

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
	/*UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));
	
	ProductId = TextReader->ReadFile("Credentials/ProductId.txt");
	SandboxId = TextReader->ReadFile("Credentials/SandboxId.txt");
	DeploymentId = TextReader->ReadFile("Credentials/DeploymentId.txt");
	ClientId = TextReader->ReadFile("Credentials/ClientId.txt");
	ClientSecret = TextReader->ReadFile("Credentials/ClientSecretId.txt");*/
	//UE_LOG(UEOSLog, Log, TEXT("HERE"));
}
