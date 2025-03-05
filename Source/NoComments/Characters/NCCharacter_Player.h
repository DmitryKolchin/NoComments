// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "NCCharacter_Base.h"
#include "NCCharacter_Player.generated.h"

class UCombatComponent;

UCLASS()
class NOCOMMENTS_API ANCCharacter_Player : public ANCCharacter_Base, public IGenericTeamAgentInterface
{
	GENERATED_BODY()


public:
	ANCCharacter_Player();
	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	// ~IGenericTeamAgentInterface

	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCombatComponent* CombatComponent;

	UPROPERTY()
	FGenericTeamId TeamId;

public:
	UFUNCTION(BlueprintPure)
	UCombatComponent* GetCombatComponent() const;

};
