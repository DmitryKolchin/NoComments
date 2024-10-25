// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSettingsDataAsset.h"
#include "CombatSettingsDataAsset_AI.generated.h"

/**
 * 
 */
UCLASS()
class NOCOMMENTS_API UCombatSettingsDataAsset_AI : public UCombatSettingsDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	float AttackRange = 100.0f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Idle" )
	FFloatRange EnemyIdleTimeRange = FFloatRange( 1.f, 3.f );

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Block" )
	FFloatRange EnemyBlockTimeRange = FFloatRange( 0.5f, 1.5f );
};
