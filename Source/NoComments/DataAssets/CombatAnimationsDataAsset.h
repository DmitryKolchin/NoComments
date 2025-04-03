// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatAnimationsDataAsset.generated.h"

struct FAttackData;
/**
 * Base class for data assets that store combat related animations
 */
UCLASS( Blueprintable )
class NOCOMMENTS_API UCombatAnimationsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TArray<FAttackData> LightAttacks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttackData> MidAttacks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttackData> HeavyAttacks;
};
