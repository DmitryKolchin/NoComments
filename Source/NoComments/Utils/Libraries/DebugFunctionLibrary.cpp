// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugFunctionLibrary.h"

#include "AudioDevice.h"

void UDebugFunctionLibrary::ThrowDebugError(const FString& CallerFunction, const FString& ErrorMessage)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf( false, TEXT("Error in function %s: %s"), *CallerFunction, *ErrorMessage );
#else
	 UE_LOG( LogTemp, Fatal, TEXT("Error in function %s: %s"), *CallerFunction, *ErrorMessage);
#endif
}
