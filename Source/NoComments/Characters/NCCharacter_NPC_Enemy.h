// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCCharacter_Base.h"
#include "NCCharacter_NPC_Enemy.generated.h"

class UCombatComponent;

UCLASS()
class NOCOMMENTS_API ANCCharacter_NPC_Enemy : public ANCCharacter_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANCCharacter_NPC_Enemy();

private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UCombatComponent* CombatComponent;

public:
	UFUNCTION(BlueprintCallable)
	UCombatComponent* GetCombatComponent() const;
};
