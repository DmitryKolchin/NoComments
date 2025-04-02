// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoComments/Characters/NCCharacter_Base.h"

#include "CrowdAgent.generated.h"

class UMetahumanBuilderComponent;

UCLASS()
class NOCOMMENTS_API ACrowdAgent : public ANCCharacter_Base
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual FVector GetVelocity() const override;

private:
	UPROPERTY()
	FVector Velocity;

	UPROPERTY(VisibleAnywhere)
	TArray<ACrowdAgent*> Neighbors;

	UPROPERTY()
	FVector StartLocation;

public:
	UFUNCTION(BlueprintCallable)
	void AddOffset(const FVector& Offset);

	UFUNCTION(BlueprintCallable)
	void ResetVelocity();

	void AddNeighbor(ACrowdAgent* Neighbor);

	UFUNCTION(BlueprintCallable)
	const TArray<ACrowdAgent*>& GetNeighbors() const;

	UFUNCTION(BlueprintPure)
	FVector GetStartLocation() const;

	UFUNCTION(BlueprintPure)
	bool IsAtStartLocation() const;
};
