// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatSettingsDataAsset.generated.h"

/**
 * 
 */
UCLASS( BlueprintType )
class NOCOMMENTS_API UCombatSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	float AttackSpeed = 2.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Block" )
	float BlockWalkSpeed = 200.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Idle" )
	float IdleWalkSpeed = 300.f;
};
