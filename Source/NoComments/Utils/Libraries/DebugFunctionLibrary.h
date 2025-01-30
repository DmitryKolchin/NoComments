// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DebugFunctionLibrary.generated.h"

/**
 * Debug functions
 */
UCLASS()
class NOCOMMENTS_API UDebugFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void ThrowDebugError(const FString& CallerFunction, const FString& ErrorMessage);
};
