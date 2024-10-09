// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatAnimationsDataAsset.generated.h"

/**
 * Base class for data assets that store combat related animations
 */
UCLASS( Blueprintable )
class NOCOMMENTS_API UCombatAnimationsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Jabs" )
	TArray<TSoftObjectPtr<UAnimMontage>> RightJabMontages;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Jabs" )
	TArray<TSoftObjectPtr<UAnimMontage>> LeftJabMontages;
};
