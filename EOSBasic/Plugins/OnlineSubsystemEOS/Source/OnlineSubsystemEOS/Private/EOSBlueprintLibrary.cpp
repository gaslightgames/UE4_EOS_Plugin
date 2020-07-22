// Copyright (C) 2019-2020, Gaslight Games Ltd


#include "EOSBlueprintLibrary.h"

// Engine Includes
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

// EOS Plugin Includes
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEOSCommon.h"


bool UEOSBlueprintLibrary::RequestLoginByType(FString Id, FString Token, EEOSLoginType Type)
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(EOS_SUBSYSTEM);

	if (SubSystem != nullptr)
	{
		IOnlineIdentityPtr OnlineIdentity = SubSystem->GetIdentityInterface();

		if (OnlineIdentity != nullptr)
		{
			FString TypeAsString = UEOSCommon::EEOSLoginTypeToString(Type);
			FOnlineAccountCredentials Credentials(TypeAsString, Id, Token);
			return OnlineIdentity->Login(0, Credentials);
		}
	}

	return false;
}
