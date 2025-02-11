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
	UPROPERTY( EditDefaultsOnly, Category = "Attack" )
	float AttackSpeed = 2.f;

	UPROPERTY( EditDefaultsOnly, Category = "Attack|Damage Dealing Sphere" )
	bool DEBUG_ShowDamageDealingSphere = true;

	UPROPERTY( EditDefaultsOnly, Category="Attack|Damage Dealing Sphere" )
	float DamageDealingSphereRadius = 20.0f;

	/**
	 * Walk speed in fight mode when blocking
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Walk Speed", DisplayName="Block Walk Speed" )
	float FightModeWalkSpeed_Block = 200.f;

	/**
	 * Default walk speed in fight mode (when non-blocking)
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Walk Speed", DisplayName="Default Walk Speed" )
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

	float GetDamageDealingSphereRadius() const
	{
		return DamageDealingSphereRadius;
	}

	bool ShouldShowDamageDealingSphere() const
	{
		return DEBUG_ShowDamageDealingSphere;
	}
};
