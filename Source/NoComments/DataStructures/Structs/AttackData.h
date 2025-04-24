#pragma once

#include "CoreMinimal.h"
#include "AttackData.generated.h"

USTRUCT( BlueprintType )
struct FAttackData
{
	GENERATED_BODY()

	/** The attack animation montage for this attack */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Animation" )
	TSoftObjectPtr<UAnimMontage> AttackAnimationMontage;

	/** The name of the socket on the skeletal mesh component to attach the damage dealing component to */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Animation" )
	FName DamageDealingComponentSocketName;

	/** The damage dealt by this attack */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Damage" )
	float DamageDealt = 0.0f;

	/** The stamina cost of this attack */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Stamina" )
	float StaminaCost = 0.0f;

	static FAttackData EmptyAttackData;
};
