// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FinisherAnimationsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FFinisherInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UAnimMontage> FinisherAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UAnimMontage> FinisherHitAnimation = nullptr;

};

/**
 * 
 */
UCLASS()
class NOCOMMENTS_API UFinisherAnimationsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FFinisherInfo> FinisherAnimations;

	UPROPERTY(EditDefaultsOnly)
	float DistanceBetweenFighters = 156.f;

public:
	UFUNCTION(BlueprintPure)
	const TArray<FFinisherInfo>& GetFinisherAnimations() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Distance between fighters" ) float GetDistanceBetweenFighters() const;

	UFUNCTION(BlueprintPure)
	UPARAM( DisplayName = "Random Finisher" ) FFinisherInfo GetRandomFinisher() const;

};
