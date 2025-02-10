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
	 * All the possible anim montages for the AI to use when taunting
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Taunts" )
	TArray<TSoftObjectPtr<UAnimMontage>> TauntMontages;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Preparation", meta=(Units="Seconds") )
	float DecisionUpdateTime = 5.f;

	/**
	 * The chance that the AI will taunt the player
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="Preparation", meta=(ClampMax="1.0", ClampMin="0.0") )
	float ChanceToTaunt = 0.4f;

	UPROPERTY(BlueprintReadWrite)
	float ChanceToGoToPlayer = 0.6f;


#pragma region ATTACK SETTINGS

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack", meta=(Units="Centimeters") )
	FFloatRange CombatAreaZone = FFloatRange( 100.f, 200.f );

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	UCombatAnimationsDataAsset* CombatAnimations = nullptr;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category= "Attack|Odds", meta=(ClampMax="1.0", ClampMin="0.0") )
	float LightAttackChance = 0.6f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack|Odds", meta=(ClampMax="1.0", ClampMin="0.0") )
	float MidAttackChance = 0.3f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack|Odds", meta=(ClampMax="1.0", ClampMin="0.0") )
	float HeavyAttackChance = 0.1f;

#pragma endregion



	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Idle" )
	FFloatRange EnemyIdleTimeRange = FFloatRange( 1.f, 3.f );

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Block" )
	FFloatRange EnemyBlockTimeRange = FFloatRange( 0.5f, 1.5f );

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};

inline void UCombatSettingsDataAsset_AI::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty( PropertyChangedEvent );

	if (PropertyChangedEvent.Property->GetName() == "ChanceToTaunt")
	{
		ChanceToGoToPlayer = 1.f - ChanceToTaunt;
	}
}
