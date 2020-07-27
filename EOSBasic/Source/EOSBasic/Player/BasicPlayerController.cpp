// Copyright (C) Gaslight Games Ltd, 2019-2020


// EOS Basic Includes
#include "BasicPlayerController.h"

// Engine Includes
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

// EOS Plugin Includes
#include "OnlineSubsystemEOS.h"
//TODO: why can't this be a .h? Isn't the static method defined there?
#include "OnlineSubsystemEOSCommon.cpp"


bool ABasicPlayerController::RequestLogin( FString Id, FString Token, FString Type )
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get( EOS_SUBSYSTEM );

	if( SubSystem != nullptr )
	{
		IOnlineIdentityPtr OnlineIdentity = SubSystem->GetIdentityInterface();

		if( OnlineIdentity != nullptr )
		{
			FOnlineAccountCredentials Credentials( Type, Id, Token );
			return OnlineIdentity->Login( 0, Credentials );
		}
	}

	return false;
}
