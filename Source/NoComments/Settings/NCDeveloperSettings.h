// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NCDeveloperSettings.generated.h"

/**
 *
 */
UCLASS( Config=Game, defaultconfig )
class NOCOMMENTS_API UNCDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bDebugShowHealthAndStaminaBars = false;

public:
	bool ShouldShowHealthAndStaminaBars() const;
};
