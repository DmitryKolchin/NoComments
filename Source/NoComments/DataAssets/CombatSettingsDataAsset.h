// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatSettingsDataAsset.generated.h"

class UStunAnimationsDataAsset;
/**
 * 
 */
UCLASS( BlueprintType )
class NOCOMMENTS_API UCombatSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	/**
	 * The maximum amount of health the character can have
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Health" )
	float MaxHealth = 100.f;

	/**
	 * The maximum amount of stamina the character can have
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Stamina" )
	float MaxStamina = 100.f;

	/**
	 * Speed at which stamina is restored
	 */
	UPROPERTY( EditDefaultsOnly, Category="Stamina" )
	float StaminaRestoreRate = 0.5f;

	/**
	 * Delay after an attack before stamina starts to restore
	 */
	UPROPERTY( EditDefaultsOnly, Category="Stamina" )
	float StaminaAfterAttackRestoreDelay = 1.0f;

	/**
	 * Delay after blocking an attack before stamina starts to restore
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaAfterBlockRestoreDelay = 0.5f;

	/**
	 * Distance at which soft lock will be applied
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Soft Lock" )
	float SoftLockDistance = 500.f;

	/** Rate of the attack animations */
	UPROPERTY( EditDefaultsOnly, Category = "Attack" )
	float AttackSpeed = 2.f;

	UPROPERTY( EditDefaultsOnly, Category = "Attack|Damage Dealing Sphere" )
	bool DEBUG_ShowDamageDealingSphere = true;

	UPROPERTY( EditDefaultsOnly, Category = "Attack|Damage Dealing Sphere" )
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

	/**
	 * Animation data for stuns
	 */
	UPROPERTY( EditDefaultsOnly, Category="Taking Damage|Stun" )
	TObjectPtr<UStunAnimationsDataAsset> StunAnimationsDataAsset;

	/**
	 * Animation for knockout
	 */
	UPROPERTY( EditDefaultsOnly, Category="Taking Damage|Knockout" )
	TSoftObjectPtr<UAnimMontage> KnockoutAnimMontage = nullptr;

public:
	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Attack Speed" ) float GetAttackSpeed() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Block Walk Speed" ) float GetBlockWalkSpeed() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Idle Walk Speed" ) float GetIdleWalkSpeed() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Damage Dealing Sphere Radius" ) float GetDamageDealingSphereRadius() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Should Show Damage Dealing Sphere" ) bool ShouldShowDamageDealingSphere() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Max Health" ) float GetMaxHealth() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Max Stamina" ) float GetMaxStamina() const;

	UFUNCTION( BlueprintPure )
	UPARAM( DisplayName = "Soft Lock Distance" ) float GetSoftLockDistance() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Stamina Restore Rate" ) float GetStaminaRestoreRate() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Stamina After Attack Restore Delay" ) float GetStaminaAfterAttackRestoreDelay() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Stamina After Block Restore Delay" ) float GetStaminaAfterBlockRestoreDelay() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Knockout Animation" ) TSoftObjectPtr<UAnimMontage> GetKnockoutAnimMontage() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Stun Animations Data Asset" ) UStunAnimationsDataAsset* GetStunAnimationsDataAsset() const;

};
