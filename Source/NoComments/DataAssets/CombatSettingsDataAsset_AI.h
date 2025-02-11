// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSettingsDataAsset.h"
#include "CombatSettingsDataAsset_AI.generated.h"

class UCombatAnimationsDataAsset;
/**
 * 
 */
UCLASS()
class NOCOMMENTS_API UCombatSettingsDataAsset_AI : public UCombatSettingsDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * The distance range, in between which the AI is considered to be in combat area.
	 * If the distance to character less than MIN, the AI is considered to be too close to the player.
	 * If the distance to character is greater than MAX, the AI is considered to be too far from the player.
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Combat area" )
	FFloatRange CombatAreaZone = FFloatRange( 100.f, 200.f );
	/**
	 * All the possible anim montages for the AI to use when taunting
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Too Far From Player|Taunt" )
	TArray<TSoftObjectPtr<UAnimMontage>> TauntMontages;

	/**
     * The chance that the AI will taunt the player
     */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="Too Far From Player|Taunt", meta=(ClampMax="1.0", ClampMin="0.0") )
	float ChanceToTaunt = 0.4f;

	/**
	 * Time, while the AI will be simply moving to player before considering to taunt once more
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Too Far From Player|Idle" )
	FFloatRange IdleDecisionMakingTimeRange_MovingToPlayer = FFloatRange( 3.f, 5.f );

#pragma region ATTACK SETTINGS

	/** Data Asset with all the attack animations
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "In Combat Area|Attack" )
	UCombatAnimationsDataAsset* AttackAnimations = nullptr;

	/** Chance the AI will deliver the light attack (if it already decided to attack)
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category= "In Combat Area|Attack|Odds", meta=(ClampMax="1.0", ClampMin="0.0") )
	float LightAttackChance = 0.6f;

	/** Chance the AI will deliver the mid attack (if it already decided to attack)
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "In Combat Area|Attack|Odds", meta=(ClampMax="1.0", ClampMin="0.0") )
	float MidAttackChance = 0.3f;

	/** Chance the AI will deliver the heavy attack (if it already decided to attack)
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "In Combat Area|Attack|Odds", meta=(ClampMax="1.0", ClampMin="0.0") )
	float HeavyAttackChance = 0.1f;

	/** The chance the AI will go idle after the attack
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "In Combat Area|Idle", meta = (ClampMax = "1.0", ClampMin = "0.0") )
	float ChanceOfGoingIdleAfterAttack = 0.45f;

	/** The time AI will be idle after the attack
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "In Combat Area|Idle" )
	FFloatRange IdleDecisionMakingTimeRange_AfterAttack = FFloatRange( 1.f, 3.f );

#pragma endregion

#pragma region BLOCK SETTINGS

	/** The curve that describes the dependency of the block chance on the number of hits taken (number of hits taken is
	 * cleared each time after the block is activated)
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="In Combat Area|Block" )
	UCurveFloat* BlockChanceBasedOnTakenHits = nullptr;

	/** The time AI will be blocking
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "In Combat Area|Block" )
	FFloatRange EnemyBlockTimeRange = FFloatRange( 0.5f, 1.5f );

#pragma endregion

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="In Combat Area|Movement" )
	float ChanceToMoveWithinCombatArea = 0.5f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="In Combat Area|Movement", meta=(Units="Degrees") )
	float AngleUsedToCalculateNewPositionInCombatArea = 70.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="In Combat Area|Movement" )
	FFloatRange IdleDecisionMakingTimeRange_WithinCombatArea = FFloatRange( 0.5f, 1.5f );
};
