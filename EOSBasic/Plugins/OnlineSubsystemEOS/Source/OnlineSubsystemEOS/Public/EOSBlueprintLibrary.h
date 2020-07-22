// Copyright (C) 2019-2020, Gaslight Games Ltd

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnlineSubsystemEOSCommon.h"
#include "EOSBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEOS_API UEOSBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS Basic", meta = (DisplayName = "Request Login"))
		static bool						RequestLoginByType(FString Id, FString Token, EEOSLoginType Type);
};
