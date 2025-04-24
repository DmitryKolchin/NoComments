// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StunAnimationsDataAsset.generated.h"

/** Data structure for holding all relevant data for a stun animation
 */
USTRUCT( BlueprintType )
struct FStunAnimationData
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSoftObjectPtr<UAnimMontage> StunAnimation;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	float StunDuration = 1.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	float MinDamageRequired = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KnockBackDistance = 100.f;

	float GetStunAnimationPlayRate() const;
};

/**
 *  Data asset with all the stun animations
 */
UCLASS()
class NOCOMMENTS_API UStunAnimationsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY( EditDefaultsOnly )
	TArray<FStunAnimationData> StunAnimations;

public:
	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Stun Animations" )const TArray<FStunAnimationData>& GetStunAnimations() const;
};
