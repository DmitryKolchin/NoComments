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

private:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack", meta=(AllowPrivateAccess="true") )
	float AttackSpeed = 2.f;

	/**
	 * Walk speed in fight mode when blocking
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Walk Speed", meta=(AllowPrivateAccess="true"), DisplayName="Block Walk Speed" )
	float FightModeWalkSpeed_Block = 200.f;

	/**
	 * Default walk speed in fight mode (when non-blocking)
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Walk Speed", meta=(AllowPrivateAccess="true"), DisplayName="Default Walk Speed" )
	float FightModeWalkSpeed_Default = 300.f;

public:
	float GetAttackSpeed() const
	{
		return AttackSpeed;
	}

	float GetBlockWalkSpeed() const
	{
		return FightModeWalkSpeed_Block;
	}

	float GetIdleWalkSpeed() const
	{
		return FightModeWalkSpeed_Default;
	}
};
