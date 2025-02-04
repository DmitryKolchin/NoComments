// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatAnimationsDataAsset.generated.h"

struct FAttackAnimationInfo;
/**
 * Base class for data assets that store combat related animations
 */
UCLASS( Blueprintable )
class NOCOMMENTS_API UCombatAnimationsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TArray<FAttackAnimationInfo> LightAttacks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttackAnimationInfo> MidAttacks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttackAnimationInfo> HeavyAttacks;
};
