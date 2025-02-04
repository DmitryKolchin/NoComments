#pragma once

#include "CoreMinimal.h"
#include "AttackAnimationInfo.generated.h"

USTRUCT( BlueprintType )
struct FAttackAnimationInfo
{
	GENERATED_BODY()

	/** The attack animation montage for this attack */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	TSoftObjectPtr<UAnimMontage> AttackAnimationMontage;

	/** The name of the socket on the skeletal mesh component to attach the damage dealing component to */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	FName DamageDealingComponentSocketName;
};
